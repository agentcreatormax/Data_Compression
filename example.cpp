#include <bits/stdc++.h>
using namespace std ;

typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
#define pb push_back
#define mp make_pair
#define int ll
#define all(a) a.begin(),a.end()

ll hcf(ll a, ll b)
{
    if(b == 0)
        return a ;
    return hcf(b, a % b) ;
}

ll lcm(ll a, ll b)
{
    ll x = max(a, b) ;
    while(true)
    {
        if((x % a == 0) && (x % b == 0))
        {
            return x ;
        }
        x++ ;
    }
}

void lessthan3()
{
    int n;
    cout<< n << endl;
}
int32_t main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    lessthan3();
    return 0 ;
}