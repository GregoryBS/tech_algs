#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include "interface.h"

class ListGraph : public IGraph
{
public:
    explicit ListGraph(int count);
    explicit ListGraph(const IGraph& graph);
    ~ListGraph();

    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual vector<int> GetNextVertices(int vertex) const override;
    virtual vector<int> GetPrevVertices(int vertex) const override;

private:
    vector<vector<int>> neighbour_lists;
};

#endif // LISTGRAPH_H
