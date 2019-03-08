#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include "BitSet.h"

//		http://www.cplusplus.com/reference/vector/vector-bool/
//	"The storage is not necessarily an array of bool values,
//	but the library implementation may optimize storage so that
//	each value is stored in a single bit."

void dumpSet(BitSet* b)
{
	for (int i = 0; i < b->sz; i++)
		{std::cout << b->set[i];}
	std::cout << std::endl;
}

void Setter(BitSet* b, int i)
{
	std::cout << "set bit " << i << std::endl;
	setBit(b, i); dumpSet(b);
}

void Freer(BitSet* b, int i)
{
	std::cout << "free bit " << i << std::endl;
	freeItem(b, i); dumpSet(b);
}

void Allocer(BitSet* b)
{
	std::cout << "alloc an item" << std::endl;
	allocItem(b); dumpSet(b);
}

void Clearer(BitSet* b, int i)
{
	std::cout << "clear bit " << i << std::endl;
	clearBit(b, i); dumpSet(b);
}

void Filler(int max)
{
	std::cout << "Filling a bit set with " << max << " bits..." << std::endl;
	BitSet* flr = allocBitSet(max);
	for(int i = 0; i < max; i++)
		{allocItem(flr);}
	std::cout << "the size of this set is <" << sizeof(flr->set) * 8 << "> bits, says sizeof()." << std::endl;	
	dumpSet(flr);
	std::cout << std::endl;
	delete flr;
}

int main()
{
	using std::cout;
	using std::endl;

  //try reading 64 bits
	Filler(64);

  //test reading a huge number of bits
	Filler(512);

	BitSet* x = allocBitSet(5);
	dumpSet(x);	//make and dump a bitset
	Setter(x,2); //verify set works
	Allocer(x); Allocer(x); Allocer(x); //verify alloc works
	Freer(x,2); Allocer(x); //verify we can free, then alloc
	Setter(x,4); Allocer(x); //verify we can alternate alloc/set
	Clearer(x,1); Allocer(x); //verify we can clear, then alloc
	Clearer(x,3); //Freer(x,1); //clear and free some bits

	cout << endl;
  //test writing
	writeBitSet("blah.txt", x);
	cout << "write bit set" << endl;
  //test reading into properly sized bitset
	BitSet* w = allocBitSet(5);
	readBitSet("blah.txt", w);
	cout << "read bit set" << endl;
	dumpSet(w);
  //test reading into oversized bitset
	BitSet* y = allocBitSet(7);
	readBitSet("blah.txt", y);
	cout << "read oversized bit set" << endl;
	dumpSet(y);
  //test reading into undersized bitset
	BitSet* z = allocBitSet(4);
	readBitSet("blah.txt", z);
	cout << "read undersized bit set" << endl;
	dumpSet(z);
  //free memory!!
	delete w;
	delete z;
	delete y;
	delete x;
}