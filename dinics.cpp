#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Dinic {
  struct Edge {
    int to;
    long long cap;
    long long flow;
    int rev; // Index of the reverse edge in adj[to]
  };

  const long long INF = 1e18;
  int n;
  vector<vector<Edge>> adj;
  vector<int> level;
  vector<int> ptr;

  // Initialize with number of vertices
  Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}

  // Add directed edge u -> v with capacity 'cap'
  void add_edge(int u, int v, long long cap) {
    adj[u].push_back({v, cap, 0, (int)adj[v].size()});
    adj[v].push_back({u, 0, 0, (int)adj[u].size() - 1}); // Residual edge
  }

  bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    level[s] = 0;
    queue<int> q;
    q.push(s);

    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (const auto &e : adj[u]) {
        if (e.cap - e.flow > 0 && level[e.to] == -1) {
          level[e.to] = level[u] + 1;
          q.push(e.to);
        }
      }
    }
    return level[t] != -1;
  }

  long long dfs(int u, int t, long long pushed) {
    if (pushed == 0 || u == t)
      return pushed;

    for (int &cid = ptr[u]; cid < (int)adj[u].size(); ++cid) {
      auto &e = adj[u][cid];
      int tr = e.to;

      if (level[u] + 1 != level[tr] || e.cap - e.flow == 0)
        continue;

      long long tr_pushed = dfs(tr, t, min(pushed, e.cap - e.flow));
      if (tr_pushed == 0)
        continue;

      e.flow += tr_pushed;
      adj[tr][e.rev].flow -= tr_pushed;
      return tr_pushed;
    }
    return 0;
  }

  // Compute maximum flow from s to t
  long long max_flow(int s, int t) {
    long long flow = 0;
    while (bfs(s, t)) {
      fill(ptr.begin(), ptr.end(), 0);
      while (long long pushed = dfs(s, t, INF)) {
        flow += pushed;
      }
    }
    return flow;
  }
};

int main() {
  int num_nodes = 4;
  int source = 0, sink = 3;

  Dinic dinic(num_nodes);

  // Graph structure:
  // 0 -> 1 (cap 10), 0 -> 2 (cap 10)
  // 1 -> 2 (cap 2),  1 -> 3 (cap 4), 2 -> 3 (cap 8)
  dinic.add_edge(0, 1, 10);
  dinic.add_edge(0, 2, 10);
  dinic.add_edge(1, 2, 2);
  dinic.add_edge(1, 3, 4);
  dinic.add_edge(2, 3, 8);

  long long total_flow = dinic.max_flow(source, sink);
  cout << "Maximum Flow: " << total_flow << "\n"; // Output: 12

  return 0;
}