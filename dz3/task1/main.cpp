#include <iostream>
#include <cassert>
#include <climits>
#include <vector>
#include <set>

using namespace std;

class ListGraph
{
public:
    explicit ListGraph(int count);
    ~ListGraph();

    virtual void AddEdge(int from, int to, int length);
    virtual int VerticesCount() const;
    virtual vector<pair<int, int>> GetNextVertices(int vertex) const;

private:
    vector<vector<pair<int, int>>> neighbour_lists;
};

ListGraph::ListGraph(int count) : neighbour_lists(count)
{

}

ListGraph::~ListGraph()
{

}

void ListGraph::AddEdge(int from, int to, int length)
{
    assert(from >= 0 && to < neighbour_lists.size());
    pair<int, int> way;
    way.second = length;
    way.first = to;
    neighbour_lists[from].push_back(way);
    way.first = from;
    neighbour_lists[to].push_back(way);
}

int ListGraph::VerticesCount() const
{
    return neighbour_lists.size();
}

vector<pair<int, int>> ListGraph::GetNextVertices(int vertex) const
{
    assert(vertex >= 0 && vertex < neighbour_lists.size());
    vector<pair<int, int>> result = {};
    for (size_t i = 0; i < neighbour_lists[vertex].size(); i++)
        result.push_back(neighbour_lists[vertex][i]);
    return result;
}

int min_len_path(const ListGraph &graph, int begin, int end, int k)
{
    vector<int> distance(graph.VerticesCount(), INT_MAX);
    set<int> queue;
    queue.insert(begin);
    distance[begin] = 0;
    int tmp = 1;
    bool flag = true;
    while (!queue.empty() && flag)
    {
	int count = 0;
	while (tmp > 0)
	{
	    int current = *(queue.begin());
	    queue.erase(queue.begin());
	    vector<pair<int, int>> neighbours = graph.GetNextVertices(current);
	    for (int i = 0; i < neighbours.size(); i++)
	    {
		int vertex = neighbours[i].first, len = neighbours[i].second;
		if (distance[vertex] > distance[current] + len)
		{
		    queue.erase(vertex);
		    distance[vertex] = distance[current] + len;
		    queue.insert(vertex);
		    count++;
		}
	    }
	    tmp--;
	}
	tmp = count;
	k--;
	if (k == 0)
	    flag = false;
    }
    if (flag)
		return distance[end];
    else
		return -1;
}

int main(void)
{
    int n = 0, m = 0, k = 0, s = 0, f = 0;
    cin >> n >> m >> k >> s >> f;
    assert(n > 0);
    ListGraph graph(n);
    int si = 0, fi = 0, pi = 0;
    for (int i = 0; i < m; i++)
    {
        cin >> si >> fi >> pi;
        graph.AddEdge(si, fi, pi);
    }
    int result = min_len_path(graph, s, f, k);
    cout << result << endl;
    return 0;
}