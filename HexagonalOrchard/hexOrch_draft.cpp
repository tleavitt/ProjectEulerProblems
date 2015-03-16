/*PE #351*/

#include <iostream>
#include <vector> 
#include <cstring>
#include <ctime>
#include "boost/rational.hpp"
#include "myHeader.hpp"

using namespace std;
//using namespace::boost;

#define BUCKETS_SIZE 1E6
#define BIGN 1E6

//H(n) seems to go at least as n^2,
// n^2 = O(H(n))
//
//Hexagonal numbers: 3n^2 - 3n + 1

typedef unsigned int uI;
typedef unsigned long ulong ;

// typedef struct BSTreeNode {
// 	boost:rational<uI> value;
// 	BSTreeNode* lessTree;
// 	BSTreeNode* greaterTree;
// } Node;
//wait, damn. BST won't work well if we're adding nodes in increasing order...
//Hash table? or Red/Black tree?

static int hash(uI num, uI denom, int nbuckets) {
   const unsigned long MULTIPLIER = 2630849305L; // magic prime number
   unsigned long hashcode = num * MULTIPLIER + denom;
   return hashcode % nbuckets;
}

static int hashBySize(uI num, uI denom, int nbuckets) {
	return num * nbuckets/denom;
}

static inline int hashBySize(boost::rational<uI>* r, uI nbuckets) {
	return r->numerator() * (nbuckets - 1)/r->denominator();
}

struct Node {
	boost::rational<uI> value;
	Node* next;
	Node(boost::rational<uI>* r) {
		memcpy(&(this->value), r, sizeof(boost::rational<uI>));
		this->next = NULL;
	}
};

//returns true if toCheck does not already appear in buckets.
//in this case, toCheck is added to buckets.
//otherwise, returns false and does not modify buckets.
bool check_and_append(boost::rational<uI> * toCheck, Node ** buckets, uI size) {
	if (buckets[hashBySize(toCheck, size)] == NULL) {
		buckets[hashBySize(toCheck, size)] = new Node(toCheck);
		return true;
	}

	//otherwise, we need to loop over the linked list and check all its entries
	Node * cur = buckets[hashBySize(toCheck, size)];
	while (true) {
		if (cur->value == *toCheck) return false;
		if (cur->next == NULL) break;
		else cur = cur->next;
	}

	cur->next = new Node(toCheck);
	return true;

}

int computeH(int N) {
	
	Node ** buckets = (Node**)calloc(BUCKETS_SIZE, sizeof(void*));

	unsigned long H = (N - 1)/2 * 6;
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
	boost::rational<uI> cur;

	//discount: case when num = 0 and case when num = denom.
	while (denom <= N) {
		//handle case denom odd
		//here: num = 2*i - 1 for i = 1 to denom
		for (int num = 1; num < denom; num += 2) {
			cur.assign(num, denom);
			if (!check_and_append(&cur, buckets, BUCKETS_SIZE)) H++;
		}
		denom++;

		//handle case denom even
		//here: num = 2i for i = 1 to denom
		for (int num = 2; num < denom; num += 2) {
			cur.assign(num, denom);
			if (!check_and_append(&cur, buckets, BUCKETS_SIZE)) H++;
		}
		denom++;
	}

	return H;
}

void testCheckAndAppend() {
	Node ** buckets = (Node**)calloc(10, sizeof(void*));	
	boost::rational<uI> n = boost::rational<uI>(2, 3);

	cout << "1: " << check_and_append(&n, buckets, 10) << endl;
	cout << "2: " << check_and_append(&n, buckets, 10) << endl;
	n.assign(21, 30);
	cout << "3: " << check_and_append(&n, buckets, 10) << endl;
	cout << "4: " << check_and_append(&n, buckets, 10) << endl;
	n.assign(1);
	cout << "3: " << check_and_append(&n, buckets, 10) << endl;
}

int main(int argc, char* argv[]) {

	boost::rational<uI> r1(1, 2);

	testCheckAndAppend();
	cout << r1 << endl;
	cout << foo(1) << endl;
	cout << "Size of Rational: " << sizeof(r1) << endl;
	cout << "Size of uInt: " << sizeof(uI) << endl;

	cout << "H(2): " << computeH(2) << endl;
	return 0;
}