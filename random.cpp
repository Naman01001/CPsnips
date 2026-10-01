#include <bits/stdc++.h>
using namespace std;

// Seed once globally
mt19937_64 rng64(chrono::steady_clock::now().time_since_epoch().count());

// Helper function to get a random integer in range [l, r]
int getRandom(int l, int r) {
  uniform_int_distribution<int> uid(l, r);
  return uid(rng64);
}

// Extremely fast, O(1) without internal loops
inline int getFastRandom(int l, int r) { return l + (rng64() % (r - l + 1)); }

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int L = 0;
  int R = 100000; // Example range

  // Generates instantly
  int fast_idx = getFastRandom(L, R);

  cout << fast_idx << "\n";

  vector<int> arr = {1, 2, 3, 4, 5};
  shuffle(arr.begin(), arr.end(), rng64);

  // Example: Pick a random element
  int random_idx = getRandom(0, arr.size() - 1);
  cout << "Random element: " << arr[random_idx] << "\n";

  return 0;
}
