#include "ListGraph.h"

#include <stdexcept>

ListGraph::ListGraph(int vertices) : adj(vertices) {}

ListGraph::ListGraph(const IGraph& graph) : adj(graph.VerticesCount()) {
    int n = graph.VerticesCount();
    for (int v = 0; v < n; ++v) {
        for (int to : graph.GetNextVertices(v)) {
            AddEdge(v, to);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    if (from < 0 || from >= (int)adj.size() || to < 0 || to >= (int)adj.size())
        throw std::out_of_range("vertex index out of range");

    adj[from].push_back(to);
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    if (vertex < 0 || vertex >= (int)adj.size())
        throw std::out_of_range("vertex index out of range");

    return adj[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    if (vertex < 0 || vertex >= (int)adj.size())
        throw std::out_of_range("vertex index out of range");

    std::vector<int> prev;
    for (int i = 0; i < (int)adj.size(); ++i) {
        for (int to : adj[i]) {
            if (to == vertex) {
                prev.push_back(i);
                break;
            }
        }
    }
    return prev;
}

int ListGraph::VerticesCount() const {
    return static_cast<int>(adj.size());
}
