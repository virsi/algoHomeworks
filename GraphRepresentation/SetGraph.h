#pragma once

#include "IGraph.h"
#include <vector>
#include <unordered_set>

class SetGraph : public IGraph {
public:
    explicit SetGraph(int vertices);
    SetGraph(const IGraph& graph);
    ~SetGraph() override = default;

    void AddEdge(int from, int to) override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

    int VerticesCount() const override;

private:
    std::vector<std::unordered_set<int>> adj;
};
