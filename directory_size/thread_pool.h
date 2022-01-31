
#pragma once

#include <mutex>
#include <atomic>
#include <condition_variable>
#include <cassert>
#include <deque>
#include <vector>
#include <future>

struct ThreadPool {

    // the mutex, condition variable and deque form a single
    // thread-safe triggered queue of tasks:
    std::mutex m;
    std::condition_variable v;
    std::atomic<int> count_active;

    // note that a packaged_task<void> can store a packaged_task<R>:
    typedef std::pair<std::packaged_task<void()>, bool> Job;
    std::deque<Job> work;

    // this holds threads objects:
    std::vector<std::thread> threads;

    // queue( lambda ) will enqueue the lambda into the tasks for the threads
    // to use.  A future of the type the lambda returns is given to let you get
    // the result out.
    template<class F, class Ret = std::result_of_t<F&()>> std::future<Ret> queue(F&& f) {
        // wrap the function object into a packaged task, splitting
        // execution from the return value:
        std::packaged_task<Ret()> p(std::forward<F>(f));

        auto r = p.get_future(); // get the return value before we hand off the task
        {
            std::unique_lock<std::mutex> l(m);
            Job job {std::move(p), false};
            work.emplace_back(std::move(job)); // store the task<R()> as a task<void()>
        }
        v.notify_all(); // wake a thread to work on the task

        return r; // return the future result of the task
    }

    // start N threads in the thread pool.
    void start(std::size_t N = 1) {
        auto thread_run = [&]() {
            thread_task();
        };
        for (std::size_t i = 0; i < N; ++i) {
            threads.emplace_back(std::thread(thread_run));
        }
    }

    // finish enques a "stop the thread" message for every thread, then waits for them:
    template<class F>
    void finish(F&& ff) {
        {
            std::unique_lock<std::mutex> l(m);
            for (const auto & unused : threads) {
                Job job {ff, true};
                work.emplace_back(std::move(job));
            }
        }
        v.notify_all();
        for (auto & f : threads) {
            f.join();
        }
        threads.clear();
    }

    void wait_idle() {
        std::unique_lock<std::mutex> l(m);
        v.wait(l, [&] {return work.empty() && count_active == 0;});
    }

private:
    // the work that a worker thread does:
    void thread_task() {
        while (true) {
            // pop a task off the queue:
            Job job;
            {
                // usual thread-safe queue code:
                std::unique_lock<std::mutex> l(m);
                if (work.empty()) {
                    v.wait(l,[&] {return !work.empty();});
                }
                job = std::move(work.front());
                work.pop_front();
                count_active++;
            }
            job.first();
            count_active--;
            v.notify_all();
            if (job.second) return;
        }
    }
};
