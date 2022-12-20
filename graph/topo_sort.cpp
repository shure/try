
#include <map>
#include <set>
#include <stack>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <queue>
#include <functional>

typedef int Vertex;
typedef std::map<Vertex, std::vector<Vertex>> Graph;

template <typename Func>
void for_each_vertex(const Graph& gr, const Func& func)
{
    for (const auto& [from, to_vect] : gr) {
        func(from);
    }
}

template <typename Func>
void for_each_edge(const Graph& gr, const Func& func)
{
    for (const auto& [from, to_vect] : gr) {
        for (Vertex to : to_vect) {
            func(from, to);
        }
    }
}

void gen_dot(const Graph& gr, const std::filesystem::path& file)
{
    std::ofstream ofs(file);
    ofs << "digraph D {\n";
    ofs << "rankdir=\"LR\"\n";
    for_each_edge(gr, [&] (Vertex from, Vertex to) {
        ofs << from << " -> " << to << "\n";
    });
    ofs << "}\n";
}

std::vector<Vertex> do_topo_sort(const Graph& gr)
{
    std::vector<Vertex> order;

    std::map<Vertex, int> indegree;
    for_each_vertex(gr, [&] (Vertex v) {
        indegree[v] = 0;
    });
    for_each_edge(gr, [&] (Vertex from, Vertex to) {
        indegree[to]++;
    });
    
    std::queue<Vertex> q;
    for_each_vertex(gr, [&] (Vertex v) {
        if (indegree[v] == 0) {
            q.push(v);
        }
    });

    while (!q.empty()) {
        Vertex v = q.front();
        order.push_back(v);
        q.pop();
        if (gr.count(v)) {
            for (Vertex to : gr.at(v)) {
                indegree[to]--;
                if (indegree[to] == 0) {
                    q.push(to);
                }
            }
            
        }
    }

    return order;
}

int main()
{
    Graph gr;
    gr.insert({0, {1}});
    gr.insert({1, {2}});
    gr.insert({2, {3}});
    gr.insert({3, {4}});
    gr.insert({4, {5, 7}});
    gr.insert({5, {6}});
    gr.insert({6, {7}});

    gen_dot(gr, "graph2.dot");

    auto order = do_topo_sort(gr);
    for (Vertex v : order) {
        std::cout << v << " ";
    }
    std::cout << "\n";
}
