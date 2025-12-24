#include "ArcGraph.h"

#include <stdexcept>

ArcGraph::ArcGraph(int vertices) : n(vertices) {}

ArcGraph::ArcGraph(const IGraph& graph) : n(graph.VerticesCount()) {
    int vertices = graph.VerticesCount();
    for (int v = 0; v < vertices; ++v) {
        for (int to : graph.GetNextVertices(v)) {
            AddEdge(v, to);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    if (from < 0 || from >= n || to < 0 || to >= n)
        throw std::out_of_range("vertex index out of range");

    edges.emplace_back(from, to);
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    if (vertex < 0 || vertex >= n) throw std::out_of_range("vertex index out of range");

    std::vector<int> next;
    for (const auto &e : edges) if (e.first == vertex) next.push_back(e.second);
    return next;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    if (vertex < 0 || vertex >= n) throw std::out_of_range("vertex index out of range");

    std::vector<int> prev;
    for (const auto &e : edges) if (e.second == vertex) prev.push_back(e.first);
    return prev;
}

int ArcGraph::VerticesCount() const {
    return n;
}
