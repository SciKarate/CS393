//josh reiss
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include "header_files/block_device.hh"
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

//allocates memory for blockmap; returns ptr
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
//use GET_BIT b/c we want bools, not bits.
///1) make a char array and fill it with the block where blockmap is
///2) do GET_BIT everywhere in char array
///2-1) if bit @ index, set bool true
///2-2) else, set bool false
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
///i'll walk you through it...
void writeBlockMap(BlockDevice *bd, BlockMap* b, disk_addr_t diskLoc)
{
	char outarr[bd->m_bytesPerBlock];
	bool cont = true;
	for(int i = 0; cont == true; i++)
	{
	  //if we have run out of room & run out of bits to write, flag that.
		if(!(i < bd->m_bytesPerBlock) && !(i < b->sz))
			{cont = false;}
	  //if we still have bits to write, write them.
	  //use SET_BIT & CLEAR_BIT b/c we want to write bits, not bools.
		else if(i < b->sz)
		{
			if(b->set[i] == 1) {SET_BIT(outarr, i);}
			else {CLEAR_BIT(outarr, i);}
		}
	  //if we are out of bits to write, but still have room, write 0s.
		else {CLEAR_BIT(outarr, i);}
	}
	bd->writeBlock(diskLoc, outarr);
}

//cout the blockMap
//if we read a whole empty byte, truncate
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

//cout the entire blockMap. don't truncate.
void dumpMapFull(BlockMap* b)
{
	for (int i = 0; i < b->sz; i++)
		{std::cout << b->set[i];}
	std::cout << std::endl;
}