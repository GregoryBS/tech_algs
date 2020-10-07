#ifndef ARCGRAPH_H
#define ARCGRAPH_H

#include "interface.h"

struct Edge
{
    int from, to;
    Edge() : from(0), to(0) {}
    Edge(int f, int t) : from(f), to(t) {}
    bool operator== (const Edge& e) { return from == e.from && to == e.to; }
};

class ArcGraph : public IGraph
{
public:
    explicit ArcGraph(int count);
    explicit ArcGraph(const IGraph& graph);
    ~ArcGraph();

    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual vector<int> GetNextVertices(int vertex) const override;
    virtual vector<int> GetPrevVertices(int vertex) const override;

private:
    vector<Edge> edges;
};

#endif // ARCGRAPH_H
