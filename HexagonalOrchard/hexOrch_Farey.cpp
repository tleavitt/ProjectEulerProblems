/*PE #351*/

#include <iostream>
#include <vector> 
#include <cstring>
#include <cstdlib>
#include <ctime>
//#include "boost/rational.hpp"
#include "myHeader.hpp"

using namespace std;
//using namespace::boost;

typedef unsigned long ulong;

//NOTE: only works for positive integers
static inline bool eqMod2(ulong a, ulong b) {
	return ((a & 0x1) == (b & 0x1));
}
//H(n) seems to go at least as n^2,
// n^2 = O(H(n))
//
//Hexagonal numbers: 3n^2 - 3n + 1

//Points in odd numered rows 2*n -  occur at:
// (2*i - 1), (2*n - 1)
// for i = 1..n
//Points in even numbered rows 2n occur at:
// 2*i, (2*n)
// for i = 0..n
//If gcd(row, col) is not 1, there is potentially an
//interfering point between here and the center.
//we need that: if when dividing out a common factor from row and col,
//the results are either both even or both odd (at least once).


//Note: parity of odd numbers is preserved under division,
// (1) i.e. if n = 1 mod2, then kn = 1 mod2 for all k in Z
//The same does not apply for even numbers, i.e.
// (2) i.e. if n = 1 mod2, then there exists k such that kn = 0 mod2.
//However, if a = 1 mod2, then ab = b mod2. So if a | b, b/a = b mod2,
// (3) i.e. parity is preserved under division by an odd number

//need to find a way to compute H that doesn't involve keeping track of every point in
//the grid, that data structure is waaay too big

void nextFarey(ulong* prevs) {
	// # we have: c/d = (p+a)/(q+b)
	// # c/d is in lowest terms => p = kc - a, q = kd - b for some k
	// # difference between p/q and c/d is (bc - ad)/(d(kd - b)),
	// # we want to minimize this difference => maximize k
	// # constraint: denominator q cannot be larger than n
	// # => k < (n+b)/d


}

ulong computeH(ulong N) {

	ulong order = N/2;
	ulong H = 0;
	ulong n1 = 0, d1 = 1, n2 = 1, d2 = order;
	ulong tmp1, tmp2;

	while (d2 != 1) { //find all possible pairs of coprime integers that are less than N/2
		if (eqMod2(n2, d2))
			H += N/d2 - 1;
		else
			H += (N >> 1)/(d2) - 1;
		//now, move things along
		tmp1 = n2; tmp2 = d2;
		n2 = ((order + d1)/d2) * n2  - n1;
		d2 = ((order + d1)/d2) * d2  - d1;
		n1 = tmp1;
		d1 = tmp2;

	//	if (H % N == 0) cout << "Cur H value: " << H << endl;
	}

	H *= 12;
	H += (N/2 - 1) * 6;
	H += (N - 1) * 6;
	return H;
}

int main(int argc, char* argv[]) {
	//testIsHidden(atoi(argv[1]));
	//cout << "H(" << argv[1] << "): " << computeH(atoi(argv[1])) << endl;
	//cout << "H(1E5): " << computeH(1E8) << endl;
	for (int i = 100; i <= 1000; i += 100) {
		cout << "H(" << i << "): " << computeH(i) << endl;
	}
	return 0;
}