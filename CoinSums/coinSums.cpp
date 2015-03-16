#include <iostream>
//#include <math>
using namespace std;

int numCoins = 8;
int coins[] = {1, 2, 5, 10, 20, 50, 100, 200};

int numWays(int goal) {
	if (goal < 0) return 0;
	if (goal == 0) return 1;

	int ways = 0;

	for (int i = 0; i < numCoins; i++) {
		cout << coins[i] << endl;
		ways += numWays(goal - coins[i]);
	}

	return ways;

	
}

int main(int argc, char* argv[]) {
	cout << "Number of ways to make 2.00: " << numWays(2) << endl;
	return 0;
}

