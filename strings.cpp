#include <bits/stdc++.h>
using namespace std;
#define ll long long

// General Hashing struct 
const ll M1 = 1e9 + 7, B1 = 29;
const ll M2 = 1e9 + 9, B2 = 37;
const int MAXN = 1e6 + 5;

ll pb1[MAXN], pb2[MAXN];
bool bases_initialized = false;

void init_bases() 

{
    if (bases_initialized) return;
    pb1[0] = 1; pb2[0] = 1;
    for (int i = 1; i < MAXN; i++) 
    
    {
        pb1[i] = (pb1[i - 1] * B1) % M1;
        pb2[i] = (pb2[i - 1] * B2) % M2;
    }
    bases_initialized = true;
}

struct DoubleHash 

{
    int n;
    vector<ll> h1, h2, rh1, rh2;

    DoubleHash(const string& s) 
    
    {
        init_bases();
        n = s.length();
        h1.assign(n + 1, 0); h2.assign(n + 1, 0);
        rh1.assign(n + 1, 0); rh2.assign(n + 1, 0);

        string rev_s = s;
        reverse(rev_s.begin(), rev_s.end());

        for (int i = 0; i < n; i++) 
        
        {
            // Forward hashes
            h1[i + 1] = (h1[i] * B1 + s[i]) % M1;
            h2[i + 1] = (h2[i] * B2 + s[i]) % M2;
            
            // Reverse string hashes
            rh1[i + 1] = (rh1[i] * B1 + rev_s[i]) % M1;
            rh2[i + 1] = (rh2[i] * B2 + rev_s[i]) % M2;
        }
    }

    // 0-indexed l, r (inclusive)
    pair<ll, ll> get_hash(int l, int r) 
    
    {
        ll res1 = (h1[r + 1] - h1[l] * pb1[r - l + 1]) % M1;
        if (res1 < 0) res1 += M1;
        
        ll res2 = (h2[r + 1] - h2[l] * pb2[r - l + 1]) % M2;
        if (res2 < 0) res2 += M2;
        
        return 
        
        {res1, res2};
    }

    // Hash of substring s[l...r] read backwards
    pair<ll, ll> get_rev_hash(int l, int r) 
    
    {
        // Map original string indices to reversed string indices
        int rev_l = n - 1 - r;
        int rev_r = n - 1 - l;
        
        ll res1 = (rh1[rev_r + 1] - rh1[rev_l] * pb1[rev_r - rev_l + 1]) % M1;
        if (res1 < 0) res1 += M1;
        
        ll res2 = (rh2[rev_r + 1] - rh2[rev_l] * pb2[rev_r - rev_l + 1]) % M2;
        if (res2 < 0) res2 += M2;
        
        return 
        
        {res1, res2};
    }
    
    // Check if substring s[l...r] is a palindrome in O(1)
    bool is_palindrome(int l, int r) 
    
    {
        return get_hash(l, r) == get_rev_hash(l, r);
    }
};

// KMP
// Returns pi array: pi[i] is the length of the longest proper prefix of s[0...i] that is also a suffix of s[0...i].
vector<int> get_pi(const string& s) 

{
    int n = s.length();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) 
    
    {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) 
        
        {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) 
        
        {
            j++;
        }
        pi[i] = j;
    }
    return pi;
}

// Returns z array: z[i] is the length of the longest common prefix of s and s[i...n-1].
vector<int> get_z(const string& s) 

{
    int n = s.length();
    vector<int> z(n, 0);
    for (int i = 1, l = 0, r = 0; i < n; i++) 
    
    {
        if (i <= r) 
        
        {
            z[i] = min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) 
        
        {
            z[i]++;
        }
        if (i + z[i] - 1 > r) 
        
        {
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

// Manacher's Algorithm
// Returns p array where p[i] is the length of the palindrome centered at i.
// Uses dummy characters so actual palindromes can be easily extracted:
// Actual length = p[i]. Actual center in original string = (i - 1) / 2.
vector<int> manacher(const string& s) 

{
    string t = "^#";
    for (char c : s) 
    
    {
        t += c;
        t += '#';
    }
    t += '$';
    
    int n = t.length();
    vector<int> p(n, 0);
    int c = 0, r = 0;
    
    for (int i = 1; i < n - 1; i++) 
    {
        if (i < r) 
        {
            p[i] = min(r - i, p[2 * c - i]);
        }
        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) 
        {
            p[i]++;
        }
        if (i + p[i] > r) 
        {
            c = i;
            r = i + p[i];
        }
    }
    
    return p; // Note: You usually query from index 2 to 2*N (inclusive).
}

// Trie 
struct TrieNode 
{
    int nxt[26]; 
    int cnt_end; // How many words end at this node
    int cnt_pref; // How many words pass through this node (have this prefix)

    TrieNode() 
    {
        memset(nxt, -1, sizeof(nxt));
        cnt_end = 0;
        cnt_pref = 0;
    }
};

struct Trie 
{
    vector<TrieNode> t;

    Trie() 
    {
        t.emplace_back(); // Initialize with root node at index 0
    }

    void insert(const string& s) 
    {
        int u = 0;
        for (char c : s) 
        {
            int ch = c - 'a'; // Adjust base character if not lowercase English
            if (t[u].nxt[ch] == -1) 
            {
                t[u].nxt[ch] = t.size();
                t.emplace_back();
            }
            u = t[u].nxt[ch];
            t[u].cnt_pref++;
        }
        t[u].cnt_end++;
    }

    int count_words(const string& s) 
    {
        int u = 0;
        for (char c : s) 
        {
            int ch = c - 'a';
            if (t[u].nxt[ch] == -1) return 0;
            u = t[u].nxt[ch];
        }
        return t[u].cnt_end;
    }

    int count_prefixes(const string& s) 
    {
        int u = 0;
        for (char c : s) 
        {
            int ch = c - 'a';
            if (t[u].nxt[ch] == -1) return 0;
            u = t[u].nxt[ch];
        }
        return t[u].cnt_pref;
    }

    // Assumes the string exists in the trie. Call count_words(s) > 0 first if unsure.
    void erase(const string& s) 
    {
        int u = 0;
        for (char c : s) 
        {
            int ch = c - 'a';
            u = t[u].nxt[ch];
            t[u].cnt_pref--;
        }
        t[u].cnt_end--;
    }
};