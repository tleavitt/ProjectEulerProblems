#include <iostream>
#include <stack> 
#include <ctime>

using namespace std;

#define NUM_SHEET_TYPES 4

typedef struct Envelope {
	double prob; //probability of creating the current envelope along a particular path
	int numOnes; //number of times along the path when there was one sheet in the envelope
	int numSheets; //number of sheets currently in the envelope
	int sheets[NUM_SHEET_TYPES]; //vector of number of sheets of various sizes in the envelope.
	//sizes range from A2 (0 index) to A5 (3 index)
	int depth; // depth of current envelope in the tree, i.e. number of A5 sheets removaled
} Envelope;

void printEnvelope(Envelope* env) {
	cout << "Prob: " << env->prob << endl;
	cout << "NumOnes: " << env->numOnes << endl;
	cout << "Total numSheets: " << env->numSheets << endl;
	cout << "num of each sheet: " << endl;
	for (int size = 0; size < NUM_SHEET_TYPES; ++size) {
		cout << "Size A" << size + 2 << ": " << env->sheets[size] << endl;
	}
	cout << "Depth: " << env->depth << endl;
}

static inline int getNumSheets(Envelope* ev, int size) {
	return ev->sheets[size - 2];
}

static inline void setNumSheets(Envelope* ev, int size, int val) {
	ev->sheets[size - 2] = val;
}

void initializeEnvelope(Envelope* env) {
	env->prob = 1;
	env->numOnes = 0;
	env->numSheets = NUM_SHEET_TYPES;
	for (int size = 0; size < NUM_SHEET_TYPES; ++size) {
		env->sheets[size] = 1;
	}
	env->depth = 1;
}


//updates newEnv to be the result of choosing a sheet of size picked from oldEnv
//and performing the cutting and replacing procedure.
void makeMove(Envelope* oldEnv, Envelope* newEnv, int picked) {
	for (int size = 2; size < picked; size++) {
			setNumSheets(newEnv, size, getNumSheets(oldEnv, size));
	}

	setNumSheets(newEnv, picked, getNumSheets(oldEnv, picked) - 1);

	for (int size = picked + 1; size <= NUM_SHEET_TYPES + 1; size++) {
		setNumSheets(newEnv, size, getNumSheets(oldEnv, size) + 1);
	}

	int dNumSheets = (NUM_SHEET_TYPES + 1 + 1 - picked) - 1 - 1;
	//num sheets created - num sheets destroyed - one A5 sheet removed
	newEnv->numSheets = oldEnv->numSheets + dNumSheets;
	newEnv->prob = oldEnv->prob * getNumSheets(oldEnv, picked) / oldEnv->numSheets;
	newEnv->numOnes = (newEnv->numSheets == 1) ? oldEnv->numOnes + 1 : oldEnv->numOnes;
	newEnv->depth = oldEnv->depth + 1;
}

double computeExpectedValue() {
	//stack<Envelope> toProcess;
	Envelope toProcess[NUM_SHEET_TYPES*(1 << NUM_SHEET_TYPES)];
	//4: max number of moves per node (since we have four types: A2-A5 to choose from), i.e. max branching factor
	//16: max tree depth, since the A1 sheet contains 16 A5 subsheets (or generally, the envelope contains 2^(n-1) An sheets)
	initializeEnvelope(toProcess);
	int length = 1;
	//printEnvelope(toProcess);

	double expectedNumOnes = 0;
	while (length > 0) {
		//pop the last element in the stack
		Envelope cur = toProcess[length - 1];
		length--;

		if (cur.depth == (1 << NUM_SHEET_TYPES) - 1) {
			//means it's parent contained just a single A5 sheet.
			expectedNumOnes += cur.prob * (cur.numOnes - 1); //discounts last A5 sheet.
			// for (int i = 0; i < length; ++i)
			// {
			// 	cout << i << endl;
			// 	printEnvelope(toProcess + i);
			// }
			// cout << endl << "EV contribution: " << cur.prob * (cur.numOnes - 1) << endl;
			// printEnvelope(&cur);
			continue;
		}

		for (int i = 0; i < NUM_SHEET_TYPES; i++) {
			//loop over each possible move from cur and
			//push it on the stack
			if (cur.sheets[i] != 0) {
				makeMove(&cur, toProcess + length, i + 2);
				length++;
				// cout << i << endl;
				// printEnvelope(toProcess + length - 1);
			}
		}

	}

	return(expectedNumOnes);
}

int main(int argc, char* argv[]) {
	time_t start = clock();
	double numTimes = computeExpectedValue();
	time_t end = clock();
	cout << "Expected number of times that there is one sheet left in the envelope: " << numTimes << endl;
	cout << "Seconds Elapsed: " << difftime(end, start) << endl;
	return(0);
}