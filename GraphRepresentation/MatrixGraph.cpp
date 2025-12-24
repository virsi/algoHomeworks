#include "MatrixGraph.h"

#include <stdexcept>

MatrixGraph::MatrixGraph(int vertices) : mat(vertices, std::vector<bool>(vertices, false)) {}

MatrixGraph::MatrixGraph(const IGraph& graph) : mat(graph.VerticesCount(), std::vector<bool>(graph.VerticesCount(), false)) {
    int n = graph.VerticesCount();
    for (int v = 0; v < n; ++v) {
        for (int to : graph.GetNextVertices(v)) {
            AddEdge(v, to);
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    if (from < 0 || from >= (int)mat.size() || to < 0 || to >= (int)mat.size())
        throw std::out_of_range("vertex index out of range");

    mat[from][to] = true;
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    if (vertex < 0 || vertex >= (int)mat.size())
        throw std::out_of_range("vertex index out of range");

    std::vector<int> next;
    for (int i = 0; i < (int)mat.size(); ++i) {
        if (mat[vertex][i]) next.push_back(i);
    }
    return next;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    if (vertex < 0 || vertex >= (int)mat.size())
        throw std::out_of_range("vertex index out of range");

    std::vector<int> prev;
    for (int i = 0; i < (int)mat.size(); ++i) {
        if (mat[i][vertex]) prev.push_back(i);
    }
    return prev;
}

int MatrixGraph::VerticesCount() const {
    return static_cast<int>(mat.size());
}
