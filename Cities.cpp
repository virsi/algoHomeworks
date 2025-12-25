#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
#include <functional>

class Graph {
public:
    Graph(int vertices) : adj(vertices) {}

    void AddEdge(int from, int to, int weight) {
        if (from < 0 || from >= adj.size() || to < 0 || to >= adj.size()) {
            throw std::out_of_range("Invalid vertex index");
        }
        adj[from].push_back({to, weight});
    }

    const std::vector<std::pair<int, int>>& GetNeighbors(int vertex) const {
        if (vertex < 0 || vertex >= adj.size()) {
            throw std::out_of_range("Invalid vertex index");
        }
        return adj[vertex];
    }

    int Size() const { return adj.size(); }

private:
    std::vector<std::vector<std::pair<int, int>>> adj;
};

int DijkstraShortestPath(const Graph& graph, int start, int end) {
    int n = graph.Size();
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    dist[start] = 0;


    std::priority_queue<std::pair<int, int>,
                        std::vector<std::pair<int, int>>,
                        std::greater<std::pair<int, int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [current_dist, vertex] = pq.top();
        pq.pop();

        if (current_dist > dist[vertex]) continue;

        if (vertex == end) break;

        for (const auto& neighbor : graph.GetNeighbors(vertex)) {
            int to = neighbor.first;
            int weight = neighbor.second;

            if (dist[vertex] != std::numeric_limits<int>::max() &&
                dist[vertex] + weight < dist[to]) {
                dist[to] = dist[vertex] + weight;
                pq.push({dist[to], to});
            }
        }
    }

    return dist[end];
}

int main() {
    int n, m;
    std::cin >> n >> m;

    Graph graph(n);

    for (int i = 0; i < m; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
        graph.AddEdge(to, from, weight);
    }

    int start, end;
    std::cin >> start >> end;

    int result = DijkstraShortestPath(graph, start, end);

    if (result == std::numeric_limits<int>::max()) {
        std::cout << "0" << std::endl;
    } else {
        std::cout << result << std::endl;
    }

    return 0;
}
