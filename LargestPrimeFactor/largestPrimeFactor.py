#PE Number 3

from math import sqrt, floor

n = 600851475143

sqn = int(sqrt(n))

isprime = [True for i in range(sqn + 1)]
isprime[0] = isprime[1] = False

primes = []
for i in range(sqn + 1):
	if isprime[i]:
		primes.append(i)
		for k in range(2, sqn//i):
			isprime[i*k] = False


cur = n
largest = 1
factorization = {}
for p in primes:
	if cur % p == 0:
		largest = p
		cur /= p
		factorization[p] = 1
		while cur % p == 0:
			cur /= p
			factorization[p] += 1

if (cur != 1):
	largest = cur
	factorization[largest] = 1

print "Largest prime factor of ", n, ": ", largest
print "Factorization: "
print n, " = ", 
for p in factorization:
	print p, "^", factorization[p], " + ",

product = 1
for p in factorization:
	for i in range(factorization[p]):
		product *= p

print "Factorization is correct: ", n == product
