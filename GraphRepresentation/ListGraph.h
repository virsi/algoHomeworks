#pragma once

#include "IGraph.h"
#include <vector>

class ListGraph : public IGraph {
public:
    explicit ListGraph(int vertices);
    ListGraph(const IGraph& graph);
    ~ListGraph() override = default;

    void AddEdge(int from, int to) override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

    int VerticesCount() const override;

private:
    std::vector<std::vector<int>> adj;
};
