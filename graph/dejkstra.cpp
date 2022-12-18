
#include <map>
#include <set>
#include <stack>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <queue>
#include <functional>

typedef int Vertex;
typedef std::pair<double, Vertex> WeightVertex;
typedef std::map<Vertex, std::vector<WeightVertex>> Graph;

const double DIST_INF = std::numeric_limits<double>::max();

std::pair<double, std::vector<Vertex>> do_dejkstra(const Graph& gr, Vertex start, Vertex finish)
{
    if (start == finish) {
        return {0, {}};
    }

    std::priority_queue<WeightVertex, std::vector<WeightVertex>, std::greater<WeightVertex>> pq;
    pq.push({0.0, start});

    std::map<Vertex, double> dist;
    for (const auto& p : gr) {
        dist[p.first] = DIST_INF;
    }
    dist[start] = 0;

    std::map<Vertex, Vertex> prev;

    while (!pq.empty()) {
        Vertex u = pq.top().second;
        pq.pop();
        
        if (gr.count(u)) {
            for (const WeightVertex& v : gr.at(u)) {
                if (dist[v.second] > dist[u] + v.first) {
                    dist[v.second] = dist[u] + v.first;
                    pq.push({dist[v.second], v.second});
                    prev[v.second] = u;
                }
            }
        }
    }

    if (dist[finish] == DIST_INF) {
        return {false, {}};
    } else {
        std::vector<Vertex> path;
        while (start != finish) {
            path.push_back(finish);
            finish = prev[finish];
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
        return {dist[finish], path};
    }
}

int main()
{
    Graph gr;
    gr.insert({0, {{3.0, 1}, {1.0, 2}}});
    gr.insert({1, {{2.0, 3}, {1.0, 4}}});
    gr.insert({2, {{4.0, 1}, {4.0, 4}}});
    gr.insert({3, {{2.0, 5}, {3.0, 4}}});
    gr.insert({4, {{1.0, 5}}});
    gr.insert({5, {}});

    auto res = do_dejkstra(gr, 0, 5);
    if (res.first != DIST_INF) {
        for (Vertex v : res.second) {
            std::cout << v << "\n";
        }
    }
}
