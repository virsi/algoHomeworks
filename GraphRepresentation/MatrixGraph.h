#pragma once

#include "IGraph.h"
#include <vector>

class MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(int vertices);
    MatrixGraph(const IGraph& graph);
    ~MatrixGraph() override = default;

    void AddEdge(int from, int to) override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

    int VerticesCount() const override;

private:
    std::vector<std::vector<bool>> mat;
};
