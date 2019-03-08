#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include "block_device.hh"
typedef std::string file;

#define SET_BIT(cs, i)   ((cs[i/8]) |= (1 << (i % 8)))
#define CLEAR_BIT(cs, i) ((cs[i/8]) &= ~(1 << (i % 8)))
#define GET_BIT(cs, i) (((cs[i/8]) & (1 << (i % 8))) != 0)

class BlockMap
{
  public:
	int sz;
	std::vector<bool> set;
	BlockMap(int size)
	{
		set.resize(size, false);
		sz = size;
	}
};

BlockMap* allocBlockMap(int num_bits)
{
	BlockMap *s = new BlockMap(num_bits);
	return s;
}

//ones bit at index i
int setBit(BlockMap* b, int i)
{
	if(b->set[i] == 0)
		{b->set[i] = 1; return 0;}
	else return -1;
}

//zeroes bit at index i
int clearBit(BlockMap* b, int i)
{
	if(b->set[i] == 1)
		{b->set[i] = 0; return 0;}
	else return -1;
}

//returns value at index i
bool getBit(BlockMap* b, int i)
{
	if(b->set[i] == 0)
		{return false;}
	else return true;
}

//alloc the first available bit. return index -1 if none.
int allocItem(BlockMap* b)
{
	for(int i = 0; i < b->sz; i++)
	{
		if(b->set[i] == 0)
			{setBit(b, i); return i;}
	}
	return -1;
}

//free the bit at index i
void freeItem(BlockMap* b, int i)
	{clearBit(b,i);}

//read set from file
void readBlockMap(BlockDevice *bd, BlockMap* b, disk_addr_t diskLoc)
{
    char inarr[bd->m_bytesPerBlock];
    bd->readBlock(diskLoc, inarr);
    for(int i = 0; i < b->sz; i++)
    {
    	if(GET_BIT(inarr,i)) {b->set[i] = 1;}
    	else {b->set[i] = 0;}
    }
}

//save set to file
void writeBlockMap(BlockDevice *bd, BlockMap* b, disk_addr_t diskLoc)
{
	char outarr[bd->m_bytesPerBlock];
	bool cont = true;
	for(int i = 0; cont == true; i++)
	{
		if(!(i < bd->m_bytesPerBlock) && !(i < b->sz))
			{cont = false;}
		else if(i < b->sz)
		{
			if(b->set[i] == 1) {SET_BIT(outarr, i);}
			else {CLEAR_BIT(outarr, i);}
		}
		else {CLEAR_BIT(outarr, i);}
	}
	bd->writeBlock(diskLoc, outarr);
}

void dumpMap(BlockMap* b)
{
	int trig = 8;
	for (int i = 0; i < b->sz; i++)
	{
		std::cout << b->set[i];
		if(b->set[i] == 0) {trig--;}
		if(b->set[i] == 1) {trig = 8;}
		if(trig < 0) {i = b->sz; std::cout << "...(truncated)";}
	}
	std::cout << std::endl;
}

void dumpMapFull(BlockMap* b)
{
	for (int i = 0; i < b->sz; i++)
		{std::cout << b->set[i];}
	std::cout << std::endl;
}