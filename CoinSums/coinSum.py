#PE Problem 31

coins = [1, 2, 5, 10, 20, 50, 100, 200]

def numWays(goal):
	if (goal < 0): return 0
	if (goal == 0): return 1

	return numWays_helper(goal, goal)

#avoid duplicates by counting only partitions that are in sorted order,
#i.e. the first element is never less than any subsequent element.
def numWays_helper(goal, maxAllowed):
	if (goal < 0): return 0
	if (goal == 0): return 1
	'''or maxAllowed == 1'''

	n = 0
	for coin in coins:
		if (coin > maxAllowed): break 
		#means we've already counted this partition somewhere else in the recursion and should stop
		n += numWays_helper(goal - coin, coin)

	return n

while (True):
	goal = int(raw_input("Enter goal amt (pence): "))
	if (goal == 0): break
	print "Number of ways to make", goal, "p: ", numWays(goal)