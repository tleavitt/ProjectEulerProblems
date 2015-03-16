#PE #460
#ASSUME: Route is symmetrical and only has two intermediate vertices.
#then, route is completely determined by the first choice of point

#DAMN, THIS DOESNT WORK.

from math import log, sqrt

#time required to get from (0,1) to (D, 1), aiming initially at the point pt = (x, y) (assumes y != 1)
def T(pt, D):
	return 2*log(pt[1])/(float(pt[1])-1)*sqrt(pt[0]**2+(pt[1]-1)**2) + (float(D) - 2*pt[0])/pt[1]

def printSpacer(n):
	for i in range(n):
		print '-',
	print '' #prints a newline

def p(D): #printShortestTimeGrid

	for y in range(2, D):
		printSpacer(4*(D))
		for x in range(0, D):#range(0, D/2 + 1):
			print "|", "%0.3f" % T((x, y), D),
		print "|"

def add(a, b):
	return tuple(map(sum, zip(a, b)))


def minDir(pt, D):
	minT = T(pt, D)
	minD = (0, 0)
	deltax = [1, 0]
	deltay = [1, 0]
	if (pt[0] > 0): deltax.append(-1)
	if (pt[1] > 2): deltay.append(-1)

	dirs = [(drow, dcol) for drow in deltax for dcol in deltay]
	# for d in dirs:
	# 	print d

	for d in dirs:
		#print(add(pt, d))
		newMin = T(add(pt, d), D)
		if (newMin < minT):
			minT = newMin
			minD = d

	return minD


def greedy(D):
	cur = (0, 2)

	while (True):
		delta = minDir(cur, D)
		if (delta == (0, 0)):
			#we're at a local min
			return cur
		cur = add(cur, delta)


