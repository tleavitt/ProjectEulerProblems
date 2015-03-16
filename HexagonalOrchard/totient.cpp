#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>
using namespace std;

typedef unsigned long ulong

int phi[MAXN + 1], prime[MAXN/10], sz=0;
vector<bool> mark(MAXN + 1);

int fi(int n) {
    int result = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) result -= result / i;
        while (n % i == 0) n /= i;
    }
    if (n > 1) result -= result / n;
    return result;
}

int main(int argc, char* argv[]) {
    
   phi[1] = 1; 
   for (int i = 2; i <= MAXN; i++ ){
    if(!mark[i]){
        phi[i] = i-1;
        prime[sz++]= i;
    }
    for (int j=0; j<sz && prime[j]*i <= MAXN; j++ ){
        mark[prime[j]*i]=1;
        if(i%prime[j]==0){
            int ll = 0;int xx = i;
            while(xx%prime[j]==0)
            {
                           xx/=prime[j];
                           ll++;         
                       }
            int mm = 1;
            for(int k=0;k<ll;k++)mm*=prime[j];
            phi[i*prime[j]] = phi[xx]*mm*(prime[j]-1);
            break;
    }
        else phi[i*prime[j]] = phi[i]*(prime[j]-1 );
    }
}

     //a0 = n
     //a(k+1) = a(k) - a(k)/p 
     //==> a(number of primes that divide n) = n(1-1/p1)(1-1/p2)...