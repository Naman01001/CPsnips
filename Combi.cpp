#include <bits/stdc++.h>
using namespace std;
#define ll long long
 
// nCr calc
const ll MOD = 1e9 + 7;
const ll N = 2e5;

array<ll, N+1> fact, invfact;

ll binpow(ll a, ll b)
{
    ll res = 1;
    ll cur = a%MOD;
    while(b > 0)
    {
        if(b&1)
            res = (res * cur)%MOD;
        b = b >> 1;
        cur = (cur * cur)%MOD;
    }
 
    return res;
}

void pre()
{
    fact[0] = 1;
    for(ll i = 1; i <= N; i++)
    {
        fact[i] = (fact[i-1] * i)%MOD;
    }

    invfact[N] = binpow(fact[N], MOD-2);

    for(ll i = N-1; i >=0; i--)
    {
        invfact[i] = (invfact[i+1] * (i+1))%MOD;
    }
}

ll nCr(ll n, ll r)
{
    if(n < r || n < 0 || r < 0)   return 0;

    return ((fact[n]*invfact[r])%MOD * invfact[n-r])%MOD;
}