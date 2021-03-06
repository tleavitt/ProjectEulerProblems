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
static inline ulong mod4(ulong n) {
	return n & 0x3;
}

//Now: works on signed ints
static inline int mod4_s(int n) {
	if ( (n & (1 << 31)) == 0 ) return mod4(n);
	return ( (n & 3) == 0 ) ? 0 : 4 - (n & 3);
}

static inline ulong mod2(ulong n) {
	return n & 0x1; //look at lsb
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

ulong gcd(ulong a, ulong b) {
	while (b != 0) {
		ulong tmp = a;
		a = b;
		b = tmp % b;
	}

	return a;
}

//say gcd(a,b) = k. Can we reduce the point (a, b) to a previous one?
//Case 1:
//a, b odd:
// then k is odd, so dividing through, we get a/k, b/k are odd => YES
//Case 2:
//a odd, b even:
// then k must be odd, since odd numbers cannot have even factors.
// Suppose otherwise: then a = 0 mod2 but ka = 1 mod2 for some k. X
// Any factor of both a and b is a factor of k, and since k is odd,
// all common factors of a and b are odd.
// Thus, by (3), a and b can't be reduced to have the same parity => NO
//Case 2:
//a even, b even:
// then k is even.
// if we divide by 2, then we either get a/2, b/2 have same parity => YES, done
// OR we get a/2, b/2 have different parity.
//	wlog a == 2 mod4, b == 0 mod4
//	we have a == 2 * oddStuff, b == 2^2 * otherStuff
//	if there is some number that divides both oddStuff and otherStuff, we're golden.
//  the only way for this to NOT happen is for gcd(oddStuff, otherStuff) = 1
//	=> gcd(a, b) == gcd(2*oddStuff, 2^2*otherStuff) = 2*gcd(oddStuff, 2*otherStuff) == 2*cd(oddStuff, otherStuff) = 2
// i.e. if a and b are both 0 mod4 or both 2 mod 4, then YES.
// otherwise, YES if gcd(a, b) > 2.

//To determine this succinctly:
// Check if gcd(a, b) isnt' one.
// If a = 1 mod2
// return b = 1 mod 2
// (same for b)
// return true if a = b mod 4
// otherwise return whether gcd(a, b) > 2

bool isHidden(ulong row, ulong col) {
	ulong n = gcd(row, col);
	if (n <= 1) return false; //also handles origin

	if (mod2(row) == 1) return (mod2(col) == 1);
	if (mod2(col) == 1) return (mod2(row) == 1);

	if (mod4(row) == mod4(col)) return true;
	else return (n > 2);
}
//NOTE: above doesn't work if row == 0.

void testIsHidden(int numCols) {
	for (int col = numCols; col >= 0; col--) {
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

ulong computeH(ulong N) {

	ulong H = (N/2 - 1) * 6;
	//accounts for hits along the midline of each side,
	//i.e. when col is even and row = 0
	H += (N - 1) * 6;
	//accounts for hits along the 6 diagonals
	int count = 0;
	for (ulong col = 1; col <= N; col++) {
		for (ulong row = 2 - mod2(col); row < col; row += 2) {
			//start at 1 for odd col, 2 for even col (skip 0)
			//cout << "(" << row << ", " << col << ")" << endl;
			if (isHidden(row, col)) H += 12;
			count++;
			if (count % 100000000 == 0) cout << "Number Processed: " << count << endl;
		}
	}

	return H;
}

int main(int argc, char* argv[]) {
	while (true) {
		int col;
		cout << "Enter col name: " << endl;
		cin >> col;
		if (col == 0) break;
		printCol(col);
	}

	//testIsHidden(atoi(argv[1]));
	// cout << "H(" << argv[1] << "): " << computeH(atoi(argv[1])) << endl;
	// cout << "H(1E3): " << computeH(1E5) << endl;
	return 0;
}