#include <iostream>
#include <cassert>
#include <climits>
#include <vector>
#include <set>

#define WHITE 0
#define GRAY 1
#define BLACK 2

using namespace std;

class ListGraph
{
public:
    explicit ListGraph(int count);
    ~ListGraph();

    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual vector<int> GetNextVertices(int vertex) const override;

private:
    vector<vector<int>> neighbour_lists;
};

ListGraph::ListGraph(int count) : neighbour_lists(count)
{

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
	{
        neighbour_lists[from].push_back(to);
		neighbour_lists[to].push_back(from);
	}
}

int ListGraph::VerticesCount() const
{
    return neighbour_lists.size();
}

vector<Node> ListGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < neighbour_lists.size());
    return neighbour_lists[vertex];
}

void dfs(const ListGraph& graph, int v, vector<bool> &visited, vector<int> &colors)
{
	visited[v] = true;
	colors[v] = GRAY;
	vector<Node> neighbours = graph.GetNextVertices(v);
	for (int i = 0; i < neighbours.size(); i++)
	{
		if (colors[neighbours[i]] == GRAY)
			return false;
		if (!visited[neighbours[i]])
			dfs(graph, neighbours[i], visited, colors);
	}
	colors[v] = BLACK;
	return true;
}

int is_tree(const ListGraph& graph)
{
	vector<int> colors(graph.VerticesCount(), WHITE);
	vector<bool> visited(graph.VerticesCount(), false);
	bool flag = true;
	for (int i = 0; i < graph.VerticesCount() && flag; i++)
		if (!visited[i])
			flag = dfs(graph, i, visited, colors);
	if (flag)
		return 1;
	else
		return 0;
}

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
    int result = is_tree(graph);
    cout << result << endl;
    return 0;
}