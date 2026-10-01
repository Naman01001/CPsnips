#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

struct TwoSAT {
  int n; // Number of boolean variables
  vector<vector<int>> adj;
  vector<int> dfn, low, comp;
  vector<bool> in_stack;
  vector<bool> ans; // Solution assignment
  stack<int> st;
  int timer = 0, scc_cnt = 0;

  // Initialize with 'n' variables (indexed 0 to n-1)
  TwoSAT(int n)
      : n(n), adj(2 * n), dfn(2 * n, -1), low(2 * n), comp(2 * n, -1),
        in_stack(2 * n), ans(n) {}

  // Adds clause: (variable_u == val_u) OR (variable_v == val_v)
  // val_u = true for u, false for NOT u
  void add_clause(int u, bool val_u, int v, bool val_v) {
    adj[2 * u + !val_u].push_back(2 * v + val_v);
    adj[2 * v + !val_v].push_back(2 * u + val_u);
  }

  // Forces variable u to have truth value val_u
  void set_value(int u, bool val_u) { add_clause(u, val_u, u, val_u); }

  void tarjan(int u) {
    dfn[u] = low[u] = timer++;
    st.push(u);
    in_stack[u] = true;

    for (int v : adj[u]) {
      if (dfn[v] == -1) {
        tarjan(v);
        low[u] = min(low[u], low[v]);
      } else if (in_stack[v]) {
        low[u] = min(low[u], dfn[v]);
      }
    }

    if (low[u] == dfn[u]) {
      while (true) {
        int v = st.top();
        st.pop();
        in_stack[v] = false;
        comp[v] = scc_cnt;
        if (u == v)
          break;
      }
      scc_cnt++;
    }
  }

  // Solve and return true if a valid assignment exists
  bool solve() {
    for (int i = 0; i < 2 * n; ++i) {
      if (dfn[i] == -1)
        tarjan(i);
    }

    for (int i = 0; i < n; ++i) {
      if (comp[2 * i] == comp[2 * i + 1])
        return false; // Unsatisfiable
      ans[i] = comp[2 * i] < comp[2 * i + 1];
    }
    return true;
  }
};

int main() {
  int num_variables = 3; // Variables: x0, x1, x2
  TwoSAT solver(num_variables);

  // Clause 1: (x0 OR x1)
  solver.add_clause(0, true, 1, true);

  // Clause 2: (NOT x0 OR x2)
  solver.add_clause(0, false, 2, true);

  // Clause 3: Force x1 to be false
  solver.set_value(1, false);

  if (solver.solve()) {
    cout << "Satisfiable!\nAssignments:\n";
    for (int i = 0; i < num_variables; ++i) {
      cout << "x" << i << " = " << (solver.ans[i] ? "true" : "false") << "\n";
    }
  } else {
    cout << "Unsatisfiable!\n";
  }

  return 0;
}