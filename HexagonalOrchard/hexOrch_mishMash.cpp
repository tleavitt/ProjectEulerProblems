/*PE #351*/

#include <iostream>
#include <vector> 
#include <cstring>
#include <ctime>
#include "boost/rational.hpp"
#include "myHeader.hpp"

using namespace std;
//using namespace::boost;

#define BUCKETS_SIZE 1E8
#define BIGN 1E8

//H(n) seems to go at least as n^2,
// n^2 = O(H(n))
//
//Hexagonal numbers: 3n^2 - 3n + 1

typedef unsigned int uI;
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

static int myHash(boost::rational<ulong>* r, int nbuckets) {
   const unsigned long MULTIPLIER = 2630849305L; // magic prime number
   unsigned long hashcode = (r->denominator() ^ r->numerator()) * MULTIPLIER;
   return hashcode % nbuckets;
}

//
static int hashBySize(ulong num, ulong denom, int nbuckets) {
	return num * nbuckets/denom;
}

//
static inline int hashBySize(boost::rational<ulong>* r, ulong nbuckets) {
	return r->numerator() * (nbuckets - 1)/r->denominator();
}

struct Node {
	boost::rational<ulong> value;
	Node* next;
	Node(boost::rational<ulong>* r) {
		memcpy(&(this->value), r, sizeof(boost::rational<ulong>));
		this->next = NULL;
	}
};

//returns true if toCheck does not already appear in buckets.
//in this case, toCheck is added to buckets.
//otherwise, returns false and does not modify buckets.
bool check_and_append(boost::rational<ulong> * toCheck, Node ** buckets, ulong size) {
	int i = myHash(toCheck, size);
	if (buckets[i] == NULL) {
		buckets[i] = new Node(toCheck);
		return true;
	}

	//otherwise, we need to loop over the linked list and check all its entries
	Node * cur = buckets[i];
	while (true) {
		if (cur->value == *toCheck) return false;
		if (cur->next == NULL) break;
		else cur = cur->next;
	}

	cur->next = new Node(toCheck);
	return true;

}

int computeH(int N) {
	if (N <= 1) return 0;
	Node ** buckets = (Node**)calloc(BUCKETS_SIZE, sizeof(void*));

	unsigned long H = (N/2 - 1) * 6;
	//acounts for hits along the midline of each side,
	//i.e. when denom is even and i = 0
	H += (N - 1) * 6;
	//accounts for hits along the 6 diagonals
	cout << "H INIT: " << H << endl;
	//represents each point in the half triangle grid as an orderd pair of numbers,
	//denom corresponds to vertical height, with one unit per dot spacing
	//num corresponds to horizontal positition, with two units per dot spacing.
	//two pairs are equivalent if they have the same ratio of numerator to denominator
	int denom = 1;
	boost::rational<ulong> cur;

	//discount: case when num = 0 and case when num = denom.
	while (denom <= N) {
		//handle case denom odd
		//here: num = 2*i - 1 for i = 1 to denom
		for (int num = 1; num < denom; num += 2) {
			cur.assign(num, denom);
			if (!check_and_append(&cur, buckets, BUCKETS_SIZE)) {
				H += 12;
				if (!isHidden(num, denom)) cout << "(" << num << ", " << denom << ")" << endl;
			//we have 12-fold symmetry on these half-isocolese triangles
			}
		}
		denom++;

		//handle case denom even
		//here: num = 2i for i = 1 to denom
		for (int num = 2; num < denom; num += 2) {
			cur.assign(num, denom);
			if (!check_and_append(&cur, buckets, BUCKETS_SIZE)) {
				H += 12;
				if (!isHidden(num, denom)) cout << "(" << num << ", " << denom << ")" << endl;
			}
		}
		denom++;
	}

	return H;
}

void testCheckAndAppend() {
	Node ** buckets = (Node**)calloc(10, sizeof(void*));	
	boost::rational<ulong> n = boost::rational<ulong>(2, 3);

	cout << "1: " << check_and_append(&n, buckets, 10) << endl;
	cout << "2: " << check_and_append(&n, buckets, 10) << endl;
	n.assign(21, 30);
	cout << "3: " << check_and_append(&n, buckets, 10) << endl;
	cout << "4: " << check_and_append(&n, buckets, 10) << endl;
	n.assign(1, 1);
	cout << "5: " << check_and_append(&n, buckets, 10) << endl;
	n.assign(2, 3);
	cout << "6: " << check_and_append(&n, buckets, 10) << endl;	
	cout << "occupied: " << endl;
	for (int i = 0; i < 10; i++) {
		if (buckets[i] != NULL) cout << i << endl;
	}
}

int main(int argc, char* argv[]) {

	boost::rational<ulong> r1(1, 2);

	testCheckAndAppend();
	cout << r1 << endl;
	cout << foo(1) << endl;
	cout << "Size of Rational: " << sizeof(r1) << endl;
	cout << "Size of uInt: " << sizeof(uI) << endl;

	cout << "H(1e8): " << computeH(1E3) << endl;
	return 0;
}