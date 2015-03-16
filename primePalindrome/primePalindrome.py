def findLargestPrimePalindrome(largest):
	isPrime = [True for i in range(largest + 1)]
	isPrime[0] = isPrime[1] = False

	for i in range(2, largest + 1):
	    if (isPrime[i]):
	    	k = 2
	    	while (i*k <= largest):
	    		isPrime[i*k] = False
	    		k += 1


	for i in range(10):
		for j in range(10):
			cur = largest - 101*i - 10*j - 1
			if (isPrime[cur]): return cur


print findLargestPrimePalindrome(1000)