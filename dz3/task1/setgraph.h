#ifndef SETGRAPH_H
#define SETGRAPH_H

#include "interface.h"

class SetGraph : public IGraph
{
public:
    explicit SetGraph(int count);
    explicit SetGraph(const IGraph& graph);
    ~SetGraph();

    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual vector<int> GetNextVertices(int vertex) const override;
    virtual vector<int> GetPrevVertices(int vertex) const override;

private:
    vector<unordered_set<int>> set_array;
};

#endif // SETGRAPH_H
