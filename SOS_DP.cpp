#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct SOS_DP 
{
    // Computes the sum over all SUBMASKS for each mask.
    // dp[mask] = sum(a[submask]) for all submask in mask.
    // Time Complexity: O(N * 2^N)
    static vector<ll> submask_sum(ll n, const vector<ll>& a) 
    {
        vector<ll> dp = a;
        
        for(ll i = 0; i < n; i++) 
        {
            for(ll mask = 0; mask < (1 << n); mask++) 
            {
                // If the i-th bit is ON in the mask
                if(mask & (1 << i)) 
                {
                    // Add the value of the mask where the i-th bit is OFF
                    dp[mask] += dp[mask ^ (1 << i)];
                }
            }
        }
        
        return dp;
    }

    // Computes the sum over all SUPERMASKS for each mask.
    // dp[mask] = sum(a[supermask]) for all supermask containing mask.
    // Time Complexity: O(N * 2^N)
    static vector<ll> supermask_sum(ll n, const vector<ll>& a) 
    {
        vector<ll> dp = a;
        
        for(ll i = 0; i < n; i++) 
        {
            for(ll mask = 0; mask < (1 << n); mask++) 
            {
                // If the i-th bit is OFF in the mask
                if(!(mask & (1 << i))) 
                {
                    // Add the value of the mask where the i-th bit is ON
                    dp[mask] += dp[mask ^ (1 << i)];
                }
            }
        }
        
        return dp;
    }
};