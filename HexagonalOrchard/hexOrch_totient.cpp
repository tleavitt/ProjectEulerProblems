//PE 351

//NOTE: REQUIRES O(N) MEMORY
//I.e. if N = 1E8, I might need 800 MB of memory to run this thing...
//and compute phi[N] up to N...

//THIS ONE'S GONNA WORK

//MUST COMPUTE:
//Phi(n) for: every n <= N/2, and every odd n <= N

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>
using namespace std;

typedef unsigned long long ulong;


//Approximates the prime counting function.
//uses the fact that pi(N) ~ N/ln(N)
//overestimates by a factor of 1.5
static inline ulong pi(ulong N) {
    //underestimates the natural log of N,
    //computes it as 2/3 * log_2(N) (essentially assumes that log_2(e) == 3/2)
    for (int i = sizeof(ulong) - 1; i > 0; i--) {
        if ((1<<i) & N) return 3*N/(2*i);
    }
    return 0;
}

void updatePhi(ulong i, ulong* phi, ulong* primes, ulong* numPrimes, int N) {
    if (!phi[i]) { //means we haven't processed i before
        phi[i] = i-1;
        primes[(*numPrimes)++] = i; 
    }
    //now, use phi[i] to precompute phi for prime multiples of i
    for (int pn = 0; pn < *numPrimes && primes[pn]*i <= N; pn++) {
        ulong p = primes[pn];
        //Uses the fact that:
        //phi(p^k*a) = p^(k-1) * (p - 1) * phi(a)
        //for gcd(p, a) = 1

        ulong a = i; ulong pk_1 = 1;
        while (a % p == 0) {
            a /= p;
            pk_1 *= p;
        }
        phi[i*p] = phi[a]*pk_1*(p - 1);

    }
}

//NON-TRIVIAL RESULT
//SEE NOTES FOR MATHEMATICAL, OR AT LEAST PICTOGRAPHIC, EXPLANATION
ulong numCanSee(ulong col, ulong* phi) {
    if ((col & 0x1) == 0) { //col == 0 mod 2
        if ((col & 0x2) == 0) //col == 0 mod 4
            return phi[col/2];
        else
            return phi[col/2]/2;//col == 2 mod 4
    } else
        return phi[col]/2; //col == 1 mod 2
}

ulong numBlocked(ulong col, ulong* phi) {
    if ((col & 0x1) == 0) { //col == 0 mod 2
        if ((col & 0x2) == 0) //col == 0 mod 4
            return (col - 1)/2 - phi[col/2];
        else
            return (col - 1)/2 - phi[col/2]/2;//col == 2 mod 4
    } else
        return (col - 1)/2 - phi[col]/2; //col == 1 mod 2
}

ulong computeH(ulong N) {
    ulong H = 0;
    ulong* phi = new ulong[N+1](); //initializes to zero
    ulong* primes = new ulong[pi(N/2)];
    ulong numPrimes = 0;

    phi[1] = 1; 
    ulong col;
    for (col = 2; col <= N/2; col++) {
        updatePhi(col, phi, primes, &numPrimes, N);
        H += numBlocked(col, phi);
    }
    //At this point, we should have completely filled out phi
    //for every composite number less than N. So, if we don't find anything in 
    //phi[col], col must be prime.

    for (; col <= N; col++) {
        if (!phi[col]) phi[col] = col - 1;
        //we found a new prime, but we won't bother storing it b/c we don't need it anymore
        H += numBlocked(col, phi);
    }

    //cout << "POST LOOP: " << H << endl;
    H *= 12; //accounts for 12-fold symmetry 
    H += (N/2 - 1) * 6; //accounts for hits along the midline of each side
    H += (N - 1) * 6; //account for hits along the diagonals
    return H;
}

static inline ulong mod2(ulong n) {
    return n & 0x1; //look at lsb
}

//NOTE: only works for positive integers
static inline ulong mod4(ulong n) {
    return n & 0x3;
}

ulong gcd(ulong a, ulong b) {
    while (b != 0) {
        ulong tmp = a;
        a = b;
        b = tmp % b;
    }

    return a;
}

bool isHidden(ulong row, ulong col) {
    ulong n = gcd(row, col);
    if (n <= 1) return false; //also handles origin

    if (mod2(row) == 1) return (mod2(col) == 1);
    if (mod2(col) == 1) return (mod2(row) == 1);

    if (mod4(row) == mod4(col)) return true;
    else return (n > 2);
}

void printCol(int col) {
    for (int row = 0; row <= col; row++) {
        if (mod2(row) == mod2(col)) {
            if (isHidden(row, col)) cout << "# ";
            else cout << "- ";
        } else {
            cout << "  ";
        }
    }
    cout << "\n";
}

void testPhi(ulong N) {
    ulong* phi = new ulong[N+1](); //initializes to zero
    ulong* primes = new ulong[pi(N/2)];
    ulong numPrimes = 0;

    phi[1] = 1;
    cout << "phi(1) = " << phi[1] << "\t";

    for (int i = 2; i <= N; i++) {
        updatePhi(i, phi, primes, &numPrimes, N);
        cout << "phi(" << i << ") = " << phi[i] << "\t";
        if (i % 4 == 0) cout << endl;
    }

    cout << endl;
}

void testH(ulong N) {

    ulong* phi = new ulong[N+1](); //initializes to zero
    ulong* primes = new ulong[pi(N/2)];
    ulong numPrimes = 0;

    phi[1] = 1;

    for (int i = 2; i <= N; i++) {
        updatePhi(i, phi, primes, &numPrimes, N);
        printCol(i);
        cout << "Can See: " << numCanSee(i, phi) << endl;
        cout << "Blocked: " << numBlocked(i, phi) << endl;
    }

}

int main(int argc, char* argv[]) {
    ulong N = atoi(argv[1]);
    // testPhi(N);
    // testH(N);
    cout << "H(" << N << "): " << computeH(N) << endl;
    cout << "H(1E8) = " << computeH(1E8) << endl;
    return 0;  
}

     //a0 = n
     //a(k+1) = a(k) - a(k)/p 
     //==> a(number of primes that divide n) = n(1-1/p1)(1-1/p2)...