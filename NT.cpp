#include <bits/stdc++.h>
using namespace std;
#define ll long long

// For general do x = x0 + k*(b/gcd) and y = y0 - k*(a/gcd)
pair<ll,ll> exgcd(ll a, ll b)
{
    if(a == 0)
        return {0,1};
    auto p = exgcd(b%a,a);
    return {p.second-(b/a)*p.first,p.first};
}

const ll N = 1e6;
ll spf[N+1], eutot[N+1], mobius[N+1];

void pre()
{
    memset(spf, -1, sizeof(spf));
    
    // Initialize Euler Totient and Mobius arrays
    for(ll i = 1; i <= N; i++)
    {
        eutot[i] = i;     // Totient starts as n
        mobius[i] = 1;    // Mobius starts as 1
    }
    
    spf[0] = 1;
    spf[1] = 1;

    for(ll i = 2; i <= N; i++)
    {
        if(spf[i] == -1)
        {
            // i is a prime number
            for(ll j = i; j <= N; j += i)
            {
                // Only assign if not already touched to guarantee the SMALLEST prime factor
                if(spf[j] == -1) 
                {
                    spf[j] = i;
                }
                
                // Euler Totient logic: n *= (1 - 1/p) -> which is n -= n/p
                eutot[j] -= eutot[j] / i;
                
                // Mobius logic: flip the sign for every distinct prime factor
                mobius[j] = -mobius[j];
            }
            
            // Mobius logic: if a number is divisible by p^2, its mobius value is 0
            for(ll j = i * i; j <= N; j += (i * i))
            {
                mobius[j] = 0;
            }
        }
    }
}

// m = Root(r), TC -> O( (R - L) loglogR + mloglogm)
vector<char> SegmentedSieve(ll L, ll R)
{
    // generate all primes 
    ll lim = sqrtl(R);
    vector<char> mark(lim+1, 0);

    vector<ll> primes;

    for(ll i = 2; i <= lim; i++)
    {
        if(! mark[i])
        {
            primes.emplace_back(i);
            for(ll j = i*i; j <= lim; j+= i)
                mark[j] = true;
        }
    }

    vector<char> isPrime(R - L +1, true);
    for(ll i : primes)
    {
        for(ll j = max(i*i, (L+i-1)/(i*i)); j <= R; j += i)
        {
            isPrime[j - L] = false;
        }
    }

    if(L == 1)
        isPrime[0] = false;
    return isPrime;
}

// O(log N) Prime Factorization using SPF
vector<pair<ll, ll>> factorize(ll x)
{
    vector<pair<ll, ll>> factors;
    
    if(x <= 1 || x > N) return factors; 

    while(x > 1)
    {
        ll p = spf[x];
        ll count = 0;
        
        while(x % p == 0)
        {
            count++;
            x /= p;
        }
        
        factors.push_back({p, count});
    }
    
    return factors;
}

void dfs_divisors(int idx, ll current_divisor, const vector<pair<ll, ll>>& factors, vector<ll>& divisors)
{
    if (idx == factors.size())
    {
        divisors.push_back(current_divisor);
        return;
    }

    ll p = factors[idx].first;
    ll max_power = factors[idx].second;
    ll p_power = 1;

    // Explore taking p^0, p^1, ..., p^{max_power}
    for (ll i = 0; i <= max_power; i++)
    {
        dfs_divisors(idx + 1, current_divisor * p_power, factors, divisors);
        p_power *= p;
    }
}

//For N = 1e6, the maximum number of divisors any integer has is 240
// So here the TC is O(log(x) + d(x)) where d(x) is not very big
vector<ll> get_all_factors(ll x)
{
    vector<pair<ll, ll>> factors = factorize(x);
    vector<ll> divisors;
    
    // Safety check for 1
    if (x == 1) 
    {
        return {1};
    }

    if (factors.empty()) 
    {
        return divisors;
    }

    dfs_divisors(0, 1, factors, divisors);
    // The divisors are generated out of order, so sort them if needed
    //sort(divisors.begin(), divisors.end()); 
    return divisors;
}