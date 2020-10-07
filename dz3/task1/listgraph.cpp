#include "listgraph.h"

ListGraph::ListGraph(int count) : neighbour_lists(count)
{

}

ListGraph::ListGraph(const IGraph &graph) : neighbour_lists(graph.VerticesCount())
{
    for (int i = 0; i < graph.VerticesCount(); i++)
        neighbour_lists[i] = graph.GetNextVertices(i);
}

ListGraph::~ListGraph()
{

}

void ListGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && to < neighbour_lists.size());

    bool not_exist = true;
    for (size_t i = 0; i < neighbour_lists[from].size() && not_exist; i++)
        if (neighbour_lists[from][i] == to)
            not_exist = false;

    if (not_exist)
        neighbour_lists[from].push_back(to);
}

int ListGraph::VerticesCount() const
{
    return neighbour_lists.size();
}

vector<int> ListGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < neighbour_lists.size());
    return neighbour_lists[vertex];
}

vector<int> ListGraph::GetPrevVertices(int vertex) const
{
    vector<int> result = {};
    for (size_t i = 0; i < neighbour_lists.size(); i++)
        for (size_t j = 0; j < neighbour_lists[i].size(); j++)
            if (neighbour_lists[i][j] == vertex)
                result.push_back(i);
    return result;
}
