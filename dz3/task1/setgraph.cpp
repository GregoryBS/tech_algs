#include "setgraph.h"

SetGraph::SetGraph(int count) : set_array(count)
{

}

SetGraph::SetGraph(const IGraph &graph) : set_array(graph.VerticesCount())
{
    vector<int> vertices = {};
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        vertices = graph.GetNextVertices(i);
        for (size_t j = 0; j < vertices.size(); j++)
            set_array[i].insert(j);
    }
}

SetGraph::~SetGraph()
{

}

void SetGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && to < set_array.size());

    if (set_array[from].find(to) == set_array[from].end())
        set_array[from].insert(to);
}

int SetGraph::VerticesCount() const
{
    return set_array.size();
}

vector<int> SetGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < set_array.size());
    vector<int> result = {};
    for (auto i = set_array[vertex].begin(); i != set_array[vertex].end(); i++)
        result.push_back(i);
    return result;
}

vector<int> SetGraph::GetPrevVertices(int vertex) const
{
    vector<int> result = {};
    for (size_t i = 0; i < set_array.size(); i++)
        if (set_array[i].find(vertex) != set_array[i].end())
            result.push_back(i);
    return result;
}
