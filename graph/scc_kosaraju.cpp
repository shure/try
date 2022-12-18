
#include <map>
#include <set>
#include <stack>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <functional>

typedef int Vertex;
typedef std::map<Vertex, std::vector<Vertex>> Graph;

void gen_dot(const Graph& gr, const std::filesystem::path& file)
{
    std::ofstream ofs(file);
    ofs << "digraph D {\n";
    ofs << "rankdir=\"LR\"\n";
    for (const auto& [from, to_vect] : gr) {
        for (Vertex to : to_vect) {
            ofs << from << " -> " << to << "\n";
        }
    }
    ofs << "}\n";
}

void do_dfs(const Graph& gr, Vertex vertex, std::set<Vertex>& visited,
            const std::function<void (Vertex v)>& entry_func,
            const std::function<void (Vertex v)>& leave_func)
{
    if (visited.count(vertex)) {
        return;
    }

    entry_func(vertex);
    visited.insert(vertex);
    if (gr.count(vertex)) {
        for (Vertex to : gr.at(vertex)) {
            if (!visited.count(to)) {
                do_dfs(gr, to, visited, entry_func, leave_func);
            }
        }
    }
    leave_func(vertex);
}

void do_dfs(const Graph& gr, Vertex vertex,
            const std::function<void (Vertex v)>& entry_func,
            const std::function<void (Vertex v)>& leave_func)
{
    std::set<Vertex> visited;
    do_dfs(gr, vertex, visited, entry_func, leave_func);
}

Graph transpond(const Graph& gr) 
{
    Graph gr1;
    for (const auto& [from, to_vect] : gr) {
        for (Vertex to : to_vect) {
            gr1[to].push_back(from);
        }
    }
    return gr1;
}

std::vector<std::vector<Vertex>> do_kosaraju(const Graph& gr)
{
    std::stack<Vertex> s;

    {
        std::set<Vertex> visited;
        for (const auto& [vertex, to_vect] : gr) {
            do_dfs(gr, vertex, visited,
                   [&] (Vertex v) {},
                   [&] (Vertex v) { s.push(v); });
        }
    }
    
    Graph trans_gr = transpond(gr);
    gen_dot(trans_gr, "trans.dot");

    std::vector<std::vector<Vertex>> comps;

    {
        std::set<Vertex> visited;
        while (!s.empty()) {
            Vertex vertex = s.top();
            s.pop();
            if (visited.count(vertex)) {
                continue;
            }
            comps.push_back({});
            std::vector<Vertex>& current = comps.back();

            do_dfs(trans_gr, vertex, visited,
                   [&] (Vertex v) { current.push_back(v); },
                   [&] (Vertex v) {});
        }
    }

    return comps;
}

int main()
{
    Graph gr;
    gr.insert({0, {1}});
    gr.insert({1, {2}});
    gr.insert({2, {0, 3}});
    gr.insert({3, {4}});
    gr.insert({4, {5, 7}});
    gr.insert({5, {6}});
    gr.insert({6, {4, 7}});

    gen_dot(gr, "graph1.dot");

    do_dfs(gr, 0, [](Vertex v) {
        std::cout << "entry " << v << "\n";
    },
    [](Vertex v) {
        std::cout << "leave " << v << "\n";
    });

    auto comps = do_kosaraju(gr);
    for (const auto& comp : comps) {
        for (Vertex v : comp) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }

    Graph gr2 = transpond(gr);
    {
        std::set<Vertex> visited;
        for (const auto& [vertex, to_vect] : gr2) {
            std::cout << "doing dfs from " << vertex << "\n";
            do_dfs(gr2, vertex, visited,
                   [&] (Vertex v) {},
                   [&] (Vertex v) { std::cout << "Ver " << v << "\n"; });
        }
    }

    auto comps1 = do_kosaraju(gr2);
    for (const auto& comp : comps1) {
        for (Vertex v : comp) {
            std::cout << v << " ";
        }
        std::cout << "\n";
    }
}
