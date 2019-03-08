#!/usr/bin/python3
import sys
while True:
	for line in sys.stdin:
		lline = line.lower()
		sys.stdout.write(lline)
	sys.stdout.write("\n")
	break