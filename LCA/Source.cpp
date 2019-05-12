#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>

using namespace std;

struct IGraph {
  virtual ~IGraph() {}
  virtual void AddEdge(int, int) = 0;
  virtual bool isAncestor(int, int) = 0;
  virtual void DFS(int, int) = 0;
  virtual int LCA(int, int) = 0;
};

class CListGraph : public IGraph {
 public:
  CListGraph(int _verticesCount) {
    g.resize(_verticesCount);
    time_in.assign(_verticesCount, 0);
    time_out.assign(_verticesCount, 0);
    ancestor.resize(_verticesCount);
  }
  virtual void AddEdge(int, int);
  virtual bool isAncestor(int, int);
  virtual void DFS(int, int);
  virtual int LCA(int, int);
  vector<int> time_in;
  vector<int> time_out;
  vector<vector<int> > ancestor;

 private:
  vector<vector<int> > g;
  int depth;
};

bool CListGraph::isAncestor(int a, int b) {
  return time_in[a] <= time_in[b] && time_out[a] >= time_out[b];
}

void CListGraph::AddEdge(int from, int to) { g[from].push_back(to); }
int timer = 0;
void CListGraph::DFS(int v, int p) {
  //	static int timer = 0;
  time_in[v] = ++timer;
  ancestor[v][0] = p;
  for (int i = 1; i <= depth; ++i)
    ancestor[v][i] = ancestor[ancestor[v][i - 1]][i - 1];
  for (size_t i = 0; i < g[v].size(); ++i) {
    int to = g[v][i];
    if (to != p) DFS(to, v);
  }
  time_out[v] = ++timer;
}

int CListGraph::LCA(int a, int b) {
  if (isAncestor(a, b)) return a;
  if (isAncestor(b, a)) return b;
  for (int i = depth; i >= 0; --i)
    if (!isAncestor(ancestor[a][i], b)) a = ancestor[a][i];
  return ancestor[a][0];
}

int main() {
  int n, m, timer = 0, from, a1, a2, x, y, z, a_i_1, a_i_2, sum = 0;
  cin >> n >> m;
  CListGraph graph(n);
  for (int i = 1; i < n; i++) {
    cin >> from;
    graph.AddEdge(from, i);
  }
  int depth = 1;
  while ((1 << depth) <= n) ++depth;
  for (int i = 0; i < n; ++i) graph.ancestor[i].assign(depth + 1, 0);
  graph.DFS(0, 0);
  cin >> a1 >> a2;
  cin >> x >> y >> z;
  int v = graph.LCA(a1, a2);
  sum += v;
  a_i_1 = a2;
  a_i_2 = a1;
  for (int i = 2; i <= m; i++) {
    a1 = (x * a_i_2 + y * a_i_1 + z) % n;
    a2 = (x * a_i_1 + y * a1 + z) % n;
    int result = graph.LCA((a1 + v) % n, a2);
    sum += result;
    a_i_1 = a2;
    a_i_2 = a1;
    v = result;
  }
  cout << sum;
}