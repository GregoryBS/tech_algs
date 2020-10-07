/*
 * Задача 2. Количество различных путей.
 * Дан невзвешенный неориентированный граф. В графе может быть несколько
 * кратчайших путей между какими-то вершинами. Найдите количество различных
 * кратчайших путей между заданными вершинами. Требования: сложность O(V+E).
 */


#include <iostream>
#include <cassert>
#include <vector>
#include <queue>

using namespace std;

struct IGraph {
    virtual ~IGraph() {}

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    virtual vector<int> GetNextVertices(int vertex) const = 0;
    virtual vector<int> GetPrevVertices(int vertex) const = 0;
};

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

int min_len_path_count(const IGraph& graph, int begin, int end);

int main(void)
{
    int n = 0, edges_count = 0;
    cin >> n >> edges_count;
    assert(n > 0);
    ListGraph graph(n);
    int from = 0, to = 0;
    for (int i = 0; i < edges_count; i++)
    {
        cin >> from >> to;
        graph.AddEdge(from, to);
    }
    cin >> from >> to;
    int result = min_len_path_count(graph, from, to);
    cout << result << endl;
    return 0;
}

int min_len_path_count(const IGraph& graph, int begin, int end)
{
    assert(begin >= 0 && end >= 0);
    assert(begin < graph.VerticesCount() && end < graph.VerticesCount());
    vector<int> path_len(graph.VerticesCount(), graph.VerticesCount());
    vector<bool> visited(graph.VerticesCount(), false);
    vector<vector<int>> parents(graph.VerticesCount());
    queue<int> q;
    q.push(begin);
    path_len[begin] = 0;
    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        vector<int> neighbours = graph.GetNextVertices(current);
        for (int i = 0; i < neighbours.size(); i++)
        {
            if (path_len[neighbours[i]] >= path_len[current] + 1)
            {
                if (path_len[neighbours[i]] > path_len[current] + 1)
                {
                    parents[neighbours[i]].clear();
                    path_len[neighbours[i]] = path_len[current] + 1;
                }
                parents[neighbours[i]].push_back(current);
                if (!visited[neighbours[i]])
                {
                    q.push(neighbours[i]);
                    visited[neighbours[i]] = true;
                }
            }
        }
    }

    int result = 0;
    if (parents[end].size() > 0)
    {
        result = 1;
        q.push(end);
        while (!q.empty())
        {
            int current = q.front();
            q.pop();
            if (parents[current].size() > 0)
            {
                result += parents[current].size() - 1;
                for (int i = 0; i < parents[current].size(); i++)
                    q.push(parents[current][i]);
            }
        }
    }
    return result;
}

ListGraph::ListGraph(int count) : neighbour_lists(count) {}

ListGraph::ListGraph(const IGraph &graph) : neighbour_lists(graph.VerticesCount())
{
    for (int i = 0; i < graph.VerticesCount(); i++)
        neighbour_lists[i] = graph.GetNextVertices(i);
}

ListGraph::~ListGraph() {}

void ListGraph::AddEdge(int from, int to)
{
    assert(from >= 0 && to < neighbour_lists.size());

    bool not_exist = true;
    for (size_t i = 0; i < neighbour_lists[from].size() && not_exist; i++)
        if (neighbour_lists[from][i] == to)
            not_exist = false;

    if (not_exist)
    {
        neighbour_lists[from].push_back(to);
        neighbour_lists[to].push_back(from);
    }
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
