#include "arcgraph.h"

ArcGraph::ArcGraph(int count) : edges(0)
{

}

ArcGraph::ArcGraph(const IGraph &graph) : edges(0)
{
    vector<int> vertices = {};
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        vertices = graph.GetNextVertices(i);
        for (size_t j = 0; j < vertices.size(); j++)
        {
            Edge e(i, vertices[j]);
            edges.push_back(e);
        }
    }
}

ArcGraph::~ArcGraph()
{

}

void ArcGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && to >= 0);

    bool not_exist = true;
    Edge new_edge(from, to);
    for (size_t i = 0; i < edges.size() && not_exist; i++)
        if (new_edge == edges[i])
            not_exist = false;

    if (not_exist)
        edges.push_back(new_edge);
}

int ArcGraph::VerticesCount() const
{
    bool flag;
    vector<int> vertices = {};
    for (size_t i = 0; i < edges.size(); i++)
    {
        flag = true;
        for (size_t j = 0; j < vertices.size() && flag; j++)
            if (vertices[j] == edges[i].from)
                flag = false;
        if (flag)
            vertices.push_back(edges[i].from);
        else
            flag = true;
        for (size_t j = 0; j < vertices.size() && flag; j++)
            if (vertices[j] == edges[i].to)
                flag = false;
        if (flag)
            vertices.push_back(edges[i].to);
    }
    return vertices.size();
}

vector<int> ArcGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0);
    vector<int> result = {};
    for (size_t i = 0; i < edges.size(); i++)
        if (edges[i].from == vertex)
            result.push_back(edges[i].to);
    return result;
}

vector<int> ArcGraph::GetPrevVertices(int vertex) const
{
    assert(vertex >= 0);
    vector<int> result = {};
    for (size_t i = 0; i < edges.size(); i++)
        if (edges[i].to == vertex)
            result.push_back(edges[i].from);
    return result;
}
