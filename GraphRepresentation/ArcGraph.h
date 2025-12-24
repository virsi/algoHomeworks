#pragma once

#include "IGraph.h"
#include <vector>
#include <utility>

class ArcGraph : public IGraph {
public:
    explicit ArcGraph(int vertices);
    ArcGraph(const IGraph& graph);
    ~ArcGraph() override = default;

    void AddEdge(int from, int to) override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

    int VerticesCount() const override;

private:
    int n;
    std::vector<std::pair<int,int>> edges;
};
