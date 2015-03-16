#PE #351

# Generates pairs of coprime integers less than N.

# returns the following term an = (p, q) in the Farey sequence of order n
# whose previous two terms are a(n-1) = (c, d) and a(n-2) = (a, b)
# a(n-1) is the mediant of a(n-2) and an

def EqMod2(a, b):
	return (a & 0x1 == b & 0x1)

def nextFarey(a, b, c, d, n):
	# we have: c/d = (p+a)/(q+b)
	# c/d is in lowest terms => p = kc - a, q = kd - b for some k
	# difference between p/q and c/d is (bc - ad)/(d(kd - b)),
	# we want to minimize this difference => maximize k
	# constraint: denominator q cannot be larger than n
	# => k < (n+b)/d

	k = int((n + b)/d)
	return c, d, k*c - a, k*d - b
	# returns a(n-1), an

def computeH(N):
	#Max workable value will be N/2
	order = int(N/2)
	H = 0
	n1, d1, n2, d2 = 0, 1, 1, order

	while (n2 < d2): #find all possible pairs of coprime integers that are less than N/2
		if (EqMod2(n2, d2)):
			H += int(N/d2) - 1
			# counts the number of multiples of d2 less than N
			# all but the first corresponds to a node that is hidden from the center
		else:
			H += int(N/(2*d2)) - 1
			# in this case, only every other multiple will actually fall on the lattice.
		n1, d1, n2, d2 = nextFarey(n1, d1, n2, d2, order)

	H *= 12 #accounts for 12-fold symmetry, avoids having to do a multiplication by 12 in every line above
	H += (int(N/2) - 1) * 6;
	#accounts for hits along the midline of each side
	H += (N - 1) * 6
	# accounts for hits along each diagonal of the hexagon
	return H

def printFareySeq(n):
	a, b, c, d = 0, 1, 1, n
	print "(%d, %d)" % (a, b)

	while (c <= d): #while we have not exceeded one
		print "(%d, %d)" % (c, d)
		a, b, c, d = nextFarey(a, b, c, d, n)

# printFareySeq(10)

N = int(raw_input("Enter N: "))
print "H(%d) = %d" % (N, computeH(N))