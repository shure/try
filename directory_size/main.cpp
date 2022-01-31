
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <atomic>

#include "thread_pool.h"

ThreadPool pool;

namespace fs = boost::filesystem;

// Thread local storage
__thread uint64_t t_size, t_count;

void get_size(fs::path dir)
{
    for (auto& entry : boost::make_iterator_range(fs::directory_iterator(dir), {})) {
        fs::path child = entry.path();
        if (fs::is_symlink(child)) {
            continue;
        }
        if (fs::is_regular_file(child)) {
            t_size += fs::file_size(child);
            t_count += 1;
        } else if (fs::is_directory(child)) {
            pool.queue([child] { get_size(child); });
        }
    }
}

int main(int argc, char** argv)
{
    fs::path dir = argc > 1 ? argv[1] : ".";
    pool.start(30);

    pool.queue([dir] { get_size(dir); });

    pool.wait_idle();

    uint64_t size, count = 0;
    pool.finish([&] { size += t_size; count += t_count; });

    std::cout << "Size: " << size / 1024 << " KB\n";
    std::cout << "File count: " << count << "\n";
}
