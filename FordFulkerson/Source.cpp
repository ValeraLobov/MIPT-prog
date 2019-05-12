#include <iostream>
#include<vector>
#include <fstream>
#include <algorithm>

using namespace std;
const int INF = 1000000;

struct edgeInfo {
	int capacity;
	int flow;
	edgeInfo(int c, int f) : capacity(c), flow(f) {}
};

struct IGraph {
	virtual ~IGraph() {}
	virtual void AddEdge(int, int, int, int) = 0;
	virtual vector<pair<int, edgeInfo> > GetEdge(int) = 0;
	virtual void changeCapacity(int, int, int) = 0;
};

class CListGraph : public IGraph {
public:
	CListGraph(int _verticesCount) :
		v(_verticesCount)
	{
		g.resize(_verticesCount);
	}
	virtual void AddEdge(int, int, int, int);
	virtual vector<pair<int, edgeInfo> > GetEdge(int);
	virtual void changeCapacity(int, int, int);
	virtual void changeCapacity_inverted(int, int, int);

private:
	vector < vector < pair<int, edgeInfo> > > g;
	int v;
};

void CListGraph::changeCapacity(int start, int i, int delta)
{
	g[start][i].second.flow += delta;
}

void CListGraph::changeCapacity_inverted(int start, int to, int delta)
{
	int index = -1;
	for (int i = 0; i < g[to].size(); i++)
	{
		if (g[to][i].first == start)
		{
			index = i;
			break;
		}
	}

	if (index != -1) {
		g[to][index].second.flow -= delta;
	}
	else
	{
		edgeInfo info(0, -delta);
		g[to].push_back(make_pair(start, info));
	}
}

void CListGraph::AddEdge(int from, int to, int capacity, int flow)
{
	edgeInfo info(capacity, flow);

	g[from].push_back(make_pair(to, info));
}

vector<pair<int, edgeInfo> > CListGraph::GetEdge(int vertex)
{
	return g[vertex];
}

int findFlow(CListGraph &graph, int start, int end, int cmin, vector<bool> &visited)
{
	if (start == end) return cmin;
	visited[start] = true;
	vector<pair<int, edgeInfo> > next_vertices = graph.GetEdge(start);

	for (int i = 0; i < next_vertices.size(); i++)
	{
		int to = next_vertices[i].first;
		if (!visited[to] && next_vertices[i].second.flow < next_vertices[i].second.capacity)
		{
			int delta = findFlow(graph, to, end, min(cmin, next_vertices[i].second.capacity - next_vertices[i].second.flow), visited);
			if (delta > 0)
			{
				graph.changeCapacity(start, i, delta);
				graph.changeCapacity_inverted(start, to, delta);
				return delta;
			}
		}
	}
	return 0;
}

int solve(CListGraph &graph, int n)
{
	vector<bool>visited;
	visited.assign(n, false);
	int result = 0;
	result = findFlow(graph, 0, n - 1, INF, visited);
	int maxflow = 0;
	while (result > 0)
	{
		maxflow += result;
		fill(visited.begin(), visited.end(), false);
		result = findFlow(graph, 0, n - 1, INF, visited);
	}
	return maxflow;
}

int main()
{
	ifstream fin("maxflow.in");
	ofstream fout("maxflow.out");
	int n, m,
		edge_one, edge_two, capacity,
		result, maxflow = 0;
	fin >> n >> m;
	CListGraph graph(n);
	for (int i = 0; i < m; i++)
	{
		fin >> edge_one >> edge_two >> capacity;
		graph.AddEdge(edge_one - 1, edge_two - 1, capacity, 0);
	}
	int ans = solve(graph, n);
	fout << ans << endl;
	return 0;
}