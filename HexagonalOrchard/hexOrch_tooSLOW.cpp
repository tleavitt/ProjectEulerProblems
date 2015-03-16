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

// typedef struct BSTreeNode {
// 	boost:rational<ulong> value;
// 	BSTreeNode* lessTree;
// 	BSTreeNode* greaterTree;
// } Node;
//wait, damn. BST won't work well if we're adding nodes in increasing order...
//Hash table? or Red/Black tree?

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
			if (!check_and_append(&cur, buckets, BUCKETS_SIZE)) H += 12;
			//we have 12-fold symmetry on these half-isocolese triangles
		}
		denom++;

		//handle case denom even
		//here: num = 2i for i = 1 to denom
		for (int num = 2; num < denom; num += 2) {
			cur.assign(num, denom);
			if (!check_and_append(&cur, buckets, BUCKETS_SIZE)) H += 12;
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

	cout << "H(1e8): " << computeH(1E4) << endl;
	return 0;
}