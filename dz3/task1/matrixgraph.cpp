#include "matrixgraph.h"

MatrixGraph::MatrixGraph(int count) : matrix(count)
{
    for (int i = 0; i < count; i++)
        matrix[i].assign(count, 0);
}

MatrixGraph::MatrixGraph(const IGraph &graph) : matrix(graph.VerticesCount())
{
    vector<int> vertices = {};
    for (int i = 0; i < graph.VerticesCount(); i++)
    {
        matrix[i].assign(graph.VerticesCount(), 0);
        vertices = graph.GetNextVertices(i);
        for (int j = 0; j < vertices.size(); j++)
            matrix[i][j] = 1;
    }
}

MatrixGraph::~MatrixGraph()
{

}

void MatrixGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && to < matrix.size());

    if (matrix[from][to] == 0)
        matrix[from][to] = 1;
}

int MatrixGraph::VerticesCount() const
{
    return matrix.size();
}

vector<int> MatrixGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < matrix.size());
    vector<int> result = {};
    for (size_t i = 0; i < matrix[vertex].size(); i++)
        if (matrix[vertex][i])
            result.push_back(i);
    return result;
}

vector<int> MatrixGraph::GetPrevVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < matrix.size());
    vector<int> result = {};
    for (size_t i = 0; i < matrix.size(); i++)
        if (matrix[i][vertex])
                result.push_back(i);
    return result;
}
