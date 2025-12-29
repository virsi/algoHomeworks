// #include <stdexcept>
// #include <vector>
// #include <queue>
// #include <iostream>

// class ListGraph {
// public:
//     explicit ListGraph(int vertices);
//     ~ListGraph() = default;

//     void AddEdge(int from, int to);
//     std::vector<int> GetNextVertices(int vertex) const;
//     int VerticesCount() const;

// private:
//     std::vector<std::vector<int>> adj;
// };

// ListGraph::ListGraph(int vertices) : adj(vertices) {}

// void ListGraph::AddEdge(int from, int to) {
//     if (from < 0 || from >= (int)adj.size() || to < 0 || to >= (int)adj.size())
//         return;
//     adj[from].push_back(to);
// }

// std::vector<int> ListGraph::GetNextVertices(int vertex) const {
//     return adj[vertex];
// }

// int ListGraph::VerticesCount() const {
//     return static_cast<int>(adj.size());
// }


// int CountReachableVertices(const ListGraph& graph, int startNode) {
//     if (graph.VerticesCount() == 0) return 0;

//     std::queue<int> qu;
//     std::vector<bool> visited(graph.VerticesCount(), false);

//     qu.push(startNode);
//     visited[startNode] = true;
//     int count = 0;

//     while (!qu.empty()) {
//         int v = qu.front();
//         qu.pop();
//         count++;

//         for (int w : graph.GetNextVertices(v)) {
//             if (!visited[w]) {
//                 visited[w] = true;
//                 qu.push(w);
//             }
//         }
//     }
//     return count;
// }

// int main() {
//     int n, m;
//     if (!(std::cin >> n >> m)) return 0;

//     if (n == 0) {
//         std::cout << 0 << std::endl;
//         return 0;
//     }

//     if (m != n - 1) {
//         std::cout << 0 << std::endl;
//         return 0;
//     }

//     ListGraph graph(n);
//     for (int i = 0; i < m; ++i) {
//         int v1, v2;
//         std::cin >> v1 >> v2;
//         graph.AddEdge(v1, v2);
//         graph.AddEdge(v2, v1);
//     }

//     if (CountReachableVertices(graph, 0) == n) {
//         std::cout << 1 << std::endl;
//     } else {
//         std::cout << 0 << std::endl;
//     }

//     return 0;
// }

// #include <vector>
// #include <iostream>
// #include <queue>

// class ListGraph {
// public:
//     explicit ListGraph(int vertices) : adj(vertices) {}

//     void AddEdge(int from, int to) {
//         if (from >= 0 && from < (int)adj.size() && to >= 0 && to < (int)adj.size()) {
//             adj[from].push_back(to);
//         }
//     }

//     int VerticesCount() const { return (int)adj.size(); }

//     int GetDegree(int vertex) const {
//         return (int)adj[vertex].size();
//     }

//     const std::vector<int>& GetNextVertices(int vertex) const {
//         return adj[vertex];
//     }

// private:
//     std::vector<std::vector<int>> adj;
// };

// bool IsConnected(const ListGraph& graph) {
//     int n = graph.VerticesCount();
//     std::vector<bool> visited(n, false);

//     int start_node = -1;
//     for (int i = 0; i < n; ++i) {
//         if (graph.GetDegree(i) > 0) {
//             start_node = i;
//             break;
//         }
//     }

//     if (start_node == -1) return true;

//     std::queue<int> q;
//     q.push(start_node);
//     visited[start_node] = true;

//     while (!q.empty()) {
//         int v = q.front();
//         q.pop();

//         for (int neighbor : graph.GetNextVertices(v)) {
//             if (!visited[neighbor]) {
//                 visited[neighbor] = true;
//                 q.push(neighbor);
//             }
//         }
//     }

//     for (int i = 0; i < n; ++i) {
//         if (graph.GetDegree(i) > 0 && !visited[i]) {
//             return false;
//         }
//     }

//     return true;
// }

// bool IsEulerian(const ListGraph& graph) {
//     if (!IsConnected(graph)) return false;

//     for (int i = 0; i < graph.VerticesCount(); ++i) {
//         if (graph.GetDegree(i) % 2 != 0) {
//             return false;
//         }
//     }

//     return true;
// }

// int main() {
//     int n, m;
//     if (!(std::cin >> n >> m)) return 0;

//     ListGraph graph(n);
//     for (int i = 0; i < m; ++i) {
//         int v1, v2;
//         std::cin >> v1 >> v2;
//         graph.AddEdge(v1, v2);
//         graph.AddEdge(v2, v1);
//     }

//     if (IsEulerian(graph)) {
//         std::cout << 1 << std::endl;
//     } else {
//         std::cout << 0 << std::endl;
//     }

//     return 0;
// }


#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

const long long INF = 1e18;

struct Edge {
    int to;
    int price;
};

struct State {
    long long cost;
    int u;
    int steps;

    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

class UniverseGraph {
public:
    explicit UniverseGraph(int n) : adj(n + 1) {}

    void AddEdge(int from, int to, int price) {
        adj[from].push_back({to, price});
    }

    long long FindCheapestPath(int n, int k_limit, int start, int finish) {
        // dist[вершина][количество_шагов]
        std::vector<std::vector<long long>> dist(n + 1, std::vector<long long>(k_limit + 1, INF));
        std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

        dist[start][0] = 0;
        pq.push({0, start, 0});

        long long min_total_cost = INF;

        while (!pq.empty()) {
            State current = pq.top();
            pq.pop();

            if (current.cost > dist[current.u][current.steps]) continue;


            if (current.u == finish) {
                min_total_cost = std::min(min_total_cost, current.cost);
            }

            if (current.steps < k_limit) {
                for (const auto& edge : adj[current.u]) {
                    if (dist[current.u][current.steps] + edge.price < dist[edge.to][current.steps + 1]) {
                        dist[edge.to][current.steps + 1] = dist[current.u][current.steps] + edge.price;
                        pq.push({dist[edge.to][current.steps + 1], edge.to, current.steps + 1});
                    }
                }
            }
        }

        return (min_total_cost == INF) ? -1 : min_total_cost;
    }

private:
    std::vector<std::vector<Edge>> adj;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m, k, s, f;
    if (!(std::cin >> n >> m >> k >> s >> f)) return 0;

    UniverseGraph graph(n);
    for (int i = 0; i < m; ++i) {
        int u, v, p;
        std::cin >> u >> v >> p;
        graph.AddEdge(u, v, p);
    }

    std::cout << graph.FindCheapestPath(n, k, s, f) << std::endl;

    return 0;
}
