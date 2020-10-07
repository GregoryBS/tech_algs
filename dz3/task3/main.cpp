/*
 * Задача 3. Города.
 * Требуется отыскать самый выгодный маршрут между городами. Требования:
 * время работы O((N+M)logN), где N-количество городов, M-известных дорог
 * между ними.
 */

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

int min_len_path(const ListGraph& graph, int begin, int end);

int main(void)
{
    int n = 0, edges_count = 0;
    cin >> n >> edges_count;
    assert(n > 0);
    ListGraph graph(n);
    int from = 0, to = 0, len = 0;
    for (int i = 0; i < edges_count; i++)
    {
        cin >> from >> to >> len;
        graph.AddEdge(from, to, len);
    }
    cin >> from >> to;
    int result = min_len_path(graph, from, to);
    cout << result << endl;
    return 0;
}

int min_len_path(const ListGraph &graph, int begin, int end)
{
    vector<int> distance(graph.VerticesCount(), INT_MAX);
    set<int> queue;
    queue.insert(begin);
    distance[begin] = 0;
    while (!queue.empty())
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
            }
        }
    }
    return distance[end];
}

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
