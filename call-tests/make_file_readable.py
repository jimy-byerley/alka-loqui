#!/usr/bin/python

import sys
if len(sys.argv) < 1: exit(-1)
elif sys.argv[1] == '-h': 
	print('this simple script is used to remove line numbers at start of exemple codes take from the internet')
	exit()
filename = sys.argv[1]

f = open(filename, 'r')
lines = f.readlines()
f.close()

l = []
for line in lines:
	i = 0
	if line[i] in list('0123456789'):
		i += 1
	l.append(line[i+1:])

f = open(filename, 'w')
f.seek(0)
f.writelines(l)
f.close()
