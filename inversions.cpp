#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

// Ordered set storing pair<int, int> to handle duplicate elements
typedef tree<pair<int, int>, null_type, less<pair<int, int>>, rb_tree_tag,
             tree_order_statistics_node_update>
    ordered_set;

// ==========================================
// METHOD 1: MERGE SORT APPROACH
// ==========================================

long long mergeAndCount(vector<int> &arr, int left, int mid, int right) {
  vector<int> temp(right - left + 1);

  int i = left;
  int j = mid + 1;
  int k = 0;

  long long inv_count = 0;

  while (i <= mid && j <= right) {
    if (arr[i] <= arr[j]) {
      temp[k++] = arr[i++];
    } else {
      temp[k++] = arr[j++];
      inv_count += (mid - i + 1);
    }
  }

  while (i <= mid)
    temp[k++] = arr[i++];

  while (j <= right)
    temp[k++] = arr[j++];

  for (int p = 0; p < k; p++)
    arr[left + p] = temp[p];

  return inv_count;
}

long long mergeSortAndCount(vector<int> &arr, int left, int right) {
  long long inv_count = 0;

  if (left < right) {
    int mid = left + (right - left) / 2;

    inv_count += mergeSortAndCount(arr, left, mid);
    inv_count += mergeSortAndCount(arr, mid + 1, right);
    inv_count += mergeAndCount(arr, left, mid, right);
  }

  return inv_count;
}

long long getInversionsMergeSort(vector<int> arr) {
  if (arr.empty())
    return 0;

  return mergeSortAndCount(arr, 0, arr.size() - 1);
}

// ==========================================
// METHOD 2: FENWICK TREE (BIT) APPROACH
// ==========================================

struct FenwickTree {
  int n;
  vector<int> tree;

  FenwickTree(int n) {
    this->n = n;
    tree.assign(n + 1, 0);
  }

  void update(int i, int delta) {
    for (; i <= n; i += (i & -i))
      tree[i] += delta;
  }

  int query(int i) {
    int sum = 0;

    for (; i > 0; i -= (i & -i))
      sum += tree[i];

    return sum;
  }
};

long long getInversionsFenwick(vector<int> arr) {
  int n = arr.size();

  if (n == 0)
    return 0;

  long long inv_count = 0;

  // Coordinate compression
  vector<int> temp = arr;
  sort(temp.begin(), temp.end());

  for (int i = 0; i < n; i++) {
    arr[i] = lower_bound(temp.begin(), temp.end(), arr[i]) - temp.begin() + 1;
  }

  FenwickTree bit(n);

  // Traverse from right to left
  for (int i = n - 1; i >= 0; i--) {
    // Count elements strictly smaller than arr[i]
    inv_count += bit.query(arr[i] - 1);

    bit.update(arr[i], 1);
  }

  return inv_count;
}

// ==========================================
// METHOD 3: ORDERED SET (PBDS) APPROACH
// ==========================================

long long getInversionsOrderedSet(const vector<int> &arr) {
  ordered_set o_set;

  long long inv_count = 0;
  int n = arr.size();

  // Traverse from right to left
  for (int i = n - 1; i >= 0; i--) {

    // Number of elements strictly smaller than arr[i]
    inv_count += o_set.order_of_key({arr[i], 0});

    // Store index to allow duplicate values
    o_set.insert({arr[i], i});
  }

  return inv_count;
}
