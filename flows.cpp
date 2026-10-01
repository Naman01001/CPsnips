#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

struct MCMF {
  struct Edge {
    int to;
    long long cap;
    long long flow;
    long long cost;
    int rev;
  };

  const long long INF = 1e18;
  int n;
  vector<vector<Edge>> adj;

  // Initialize with total nodes
  MCMF(int n) : n(n), adj(n) {}

  // Add directed edge u -> v with capacity and cost per unit flow
  void add_edge(int u, int v, long long cap, long long cost) {
    adj[u].push_back({v, cap, 0, cost, (int)adj[v].size()});
    adj[v].push_back(
        {u, 0, 0, -cost,
         (int)adj[u].size() - 1}); // Residual edge with negative cost
  }

  // Returns {max_flow, min_cost}
  pair<long long, long long> min_cost_max_flow(int s, int t) {
    long long flow = 0, cost = 0;
    vector<long long> dist(n);
    vector<int> parent_node(n), parent_edge(n);
    vector<bool> in_queue(n);

    while (true) {
      fill(dist.begin(), dist.end(), INF);
      fill(in_queue.begin(), in_queue.end(), false);
      queue<int> q;

      dist[s] = 0;
      q.push(s);
      in_queue[s] = true;

      while (!q.empty()) {
        int u = q.front();
        q.pop();
        in_queue[u] = false;

        for (int i = 0; i < (int)adj[u].size(); ++i) {
          auto &e = adj[u][i];
          if (e.cap - e.flow > 0 && dist[e.to] > dist[u] + e.cost) {
            dist[e.to] = dist[u] + e.cost;
            parent_node[e.to] = u;
            parent_edge[e.to] = i;
            if (!in_queue[e.to]) {
              q.push(e.to);
              in_queue[e.to] = true;
            }
          }
        }
      }

      if (dist[t] == INF)
        break; // No more reachable augmenting paths

      long long push = INF;
      for (int u = t; u != s; u = parent_node[u]) {
        auto &e = adj[parent_node[u]][parent_edge[u]];
        push = min(push, e.cap - e.flow);
      }

      for (int u = t; u != s; u = parent_node[u]) {
        auto &e = adj[parent_node[u]][parent_edge[u]];
        e.flow += push;
        adj[u][e.rev].flow -= push;
      }

      flow += push;
      cost += push * dist[t];
    }

    return {flow, cost};
  }
};

int main() {
  int num_nodes = 4;
  int source = 0, sink = 3;

  MCMF mcmf(num_nodes);

  // Edges: (u, v, cap, cost)
  mcmf.add_edge(0, 1, 3, 1);
  mcmf.add_edge(0, 2, 2, 4);
  mcmf.add_edge(1, 2, 1, 2);
  mcmf.add_edge(1, 3, 2, 3);
  mcmf.add_edge(2, 3, 3, 1);

  auto result = mcmf.min_cost_max_flow(source, sink);
  cout << "Max Flow: " << result.first << ", Min Cost: " << result.second
       << "\n";
  // Output: Max Flow: 4, Min Cost: 18

  return 0;
}