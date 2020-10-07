#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include "interface.h"

class MatrixGraph : public IGraph
{
public:
    explicit MatrixGraph(int count);
    explicit MatrixGraph(const IGraph& graph);
    ~MatrixGraph();

    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual vector<int> GetNextVertices(int vertex) const override;
    virtual vector<int> GetPrevVertices(int vertex) const override;

private:
    vector<vector<int>> matrix;
};

#endif // MATRIXGRAPH_H
