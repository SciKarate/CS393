//josh reiss
#pragma once
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

//allocates memory for blockmap; returns ptr
BlockMap* allocBlockMap(int num_bits);
int setBit(BlockMap* b, int i);
int clearBit(BlockMap* b, int i);
bool getBit(BlockMap* b, int i);
int allocBlock(BlockMap* b);
void freeBlock(BlockMap* b, int i);
void readBlockMap(BlockDevice *bd, BlockMap* b, disk_addr_t diskLoc);
void writeBlockMap(BlockDevice *bd, BlockMap* b, disk_addr_t diskLoc);
void dumpMap(BlockMap* b);
void dumpMapFull(BlockMap* b);