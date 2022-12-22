
#include <iostream>
#include <list>
#include <string>
#include <vector>
 
typedef std::string Value;

struct Vertex {

    Vertex(const Value& value) : value(value) {
    }

    void connect(Vertex* v) {
        connections.push_back(v);
    }

    void connect(const std::initializer_list<Vertex*>& vs) {
        connections.insert(connections.end(), vs);
    }

    Value value;
    std::vector<Vertex*> connections;

    int index = -1;
    int low_link = -1;
    bool on_stack = false;
};
 
struct Graph {

    Vertex* add(const Value& value) {
        vertexes.emplace_back(value);
        return &vertexes.back();
    }

    std::list<Vertex> vertexes;
};

typedef std::vector<Vertex*> Component;

struct Tarjan {

    std::list<Component> run(Graph& graph) {
        index = 0;
        stack.clear();
        strongly_connected.clear();
        for (auto& v : graph.vertexes) {
            if (v.index == -1) {
                strongconnect(&v);
            }
        }
        return strongly_connected;
    }

    void strongconnect(Vertex* v) {
        v->index = index;
        v->low_link = index;
        ++index;
        stack.push_back(v);
        v->on_stack = true;
        for (auto w : v->connections) {
            if (w->index == -1) {
                strongconnect(w);
                v->low_link = std::min(v->low_link, w->low_link);
            }
            else if (w->on_stack) {
                v->low_link = std::min(v->low_link, w->index);
            }
        }
        if (v->low_link == v->index) {
            strongly_connected.push_back({});
            Component& c = strongly_connected.back();
            for (;;) {
                auto w = stack.back();
                stack.pop_back();
                w->on_stack = false;
                c.push_back(w);
                if (w == v) {
                    break;
                }
            }
        }
    }

    int index = 0;
    std::list<Vertex*> stack;
    std::list<Component> strongly_connected;
};
 
void print_component(const Component& comp)
{
    for (auto v : comp) {
        std::cout << ' ' << v->value;
    }
    std::cout << '\n';
}
 
int main()
{
    Graph g;
    auto andy = g.add("Andy");
    auto bart = g.add("Bart");
    auto carl = g.add("Carl");
    auto dave = g.add("Dave");
    auto earl = g.add("Earl");
    auto fred = g.add("Fred");
    auto gary = g.add("Gary");
    auto hank = g.add("Hank");
 
    andy->connect(bart);
    bart->connect(carl);
    carl->connect(andy);
    dave->connect({bart, carl, earl});
    earl->connect({dave, fred});
    fred->connect({carl, gary});
    gary->connect(fred);
    hank->connect({earl, gary, hank});
 
    Tarjan t;
    for (const Component& s : t.run(g)) {
        print_component(s);
    }

    return 0;
}
