#include <bits/stdc++.h>
#define ll long long

// --- 1. Ordered Set / PBDS Includes ---
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds;

// 1. Ordered Set Template (GNU PBDS)
// ordered_set supports:
// .find_by_order(k) : returns an iterator to the k-th smallest element (0-indexed)
// .order_of_key(k)  : returns the number of elements strictly smaller than k
template <typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

// For multiset behavior, use less_equal<T>
// Note: .erase() on less_equal removes all instances. To remove just one, use less<pair<T, int>> instead.
template <typename T>
using ordered_multiset = tree<T, null_type, less_equal<T>, rb_tree_tag, tree_order_statistics_node_update>;


// 2. Generating Subsets of a Mask (O(3^n) total)
void submask_enumeration(int n)
{
    for(int mask = 0; mask < (1 << n); mask++)
    {
      
        for(int sub = mask; sub > 0; sub = (sub - 1) & mask)
        {
            // Process the submask (Note: this loop skips sub == 0)
        }
    }
}


// 3. Custom Comparator Overloading for Structs
struct Edge 
{
    ll u;
    ll v;
    ll weight;

    // Default operator< for sorting, std::set, and std::map.
    // Must be declared 'const' at the end.
    bool operator<(const Edge& other) const
    {
        // Primary sort condition
        if(weight != other.weight)
        {
            return weight < other.weight; // Ascending order of weight
        }
        
        // Secondary sort condition (Tie-breaker)
        if(u != other.u)
        {
            return u < other.u;
        }
        
        return v < other.v;
    }
};

// Alternative: Custom external comparator struct.
// Highly useful for priority_queue when you want a Min-Heap without changing the struct's default operator<
struct MinHeapCompare 
{
    bool operator()(const Edge& a, const Edge& b) const
    {
        // For a MIN-heap, return true if 'a' is GREATER than 'b'
        return a.weight > b.weight;
    }
};

void example_usage()
{
    // Uses the operator< defined inside the struct
    vector<Edge> edges;
    sort(edges.begin(), edges.end()); 
    set<Edge> edge_set; 

    // Uses the external MinHeapCompare struct
    priority_queue<Edge, vector<Edge>, MinHeapCompare> pq;
}