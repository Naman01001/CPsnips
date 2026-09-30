#include <bits/stdc++.h>
using namespace std;
#define ll long long

//For general do x = x0 + k*(b/gcd) and y = y0 - k*(a/gcd)
pair<ll,ll> exgcd(ll a, ll b)
{
    if(a == 0)
        return {0,1};
    auto p = exgcd(b%a,a);
    return {p.second-(b/a)*p.first,p.first};
}