#include "SetGraph.h"

#include <stdexcept>

SetGraph::SetGraph(int vertices) : adj(vertices) {}

SetGraph::SetGraph(const IGraph& graph) : adj(graph.VerticesCount()) {
    int n = graph.VerticesCount();
    for (int v = 0; v < n; ++v) {
        for (int to : graph.GetNextVertices(v)) {
            AddEdge(v, to);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    if (from < 0 || from >= (int)adj.size() || to < 0 || to >= (int)adj.size())
        throw std::out_of_range("vertex index out of range");

    adj[from].insert(to);
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    if (vertex < 0 || vertex >= (int)adj.size())
        throw std::out_of_range("vertex index out of range");

    std::vector<int> next;
    next.reserve(adj[vertex].size());
    for (int to : adj[vertex]) next.push_back(to);
    return next;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    if (vertex < 0 || vertex >= (int)adj.size())
        throw std::out_of_range("vertex index out of range");

    std::vector<int> prev;
    for (int i = 0; i < (int)adj.size(); ++i) {
        if (adj[i].find(vertex) != adj[i].end()) prev.push_back(i);
    }
    return prev;
}

int SetGraph::VerticesCount() const {
    return static_cast<int>(adj.size());
}
