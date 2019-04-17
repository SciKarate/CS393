#pragma once
#include "block_device.hh"
#include <iostream>

using disk_addr_t    = int;  // signed int, so we can have negative
                             // values for, e.g., copy-on-write
class MasterBlock
{
  public:
	uint64_t magic_number = 0xDEEDFEED;
	int bytesPerBlock;
	int blockCount;
	int iNodeCnt;
	int protect = 0;
	disk_addr_t diskAddress;
	disk_addr_t diskAddress_iNodeMap;
	MasterBlock(int bytes_per_block, int number_of_blocks, disk_addr_t block_map_address)
	{
		bytesPerBlock = bytes_per_block;
		blockCount = number_of_blocks;
		diskAddress = block_map_address;
		while(number_of_blocks > 0)
		{
			number_of_blocks -= bytes_per_block;
			protect++;
		}
	  //protect iNodeMap from blockMap
		diskAddress_iNodeMap = block_map_address + protect;
	  //num inodes in a block
		int ipb = bytesPerBlock/128;
	  //num blocks currently open
		int nodeCap = blockCount - (protect + 1);
	  //how many inodes go into 1/4 of open blocks.
		iNodeCnt = (nodeCap/4) * ipb;
	  //never more than 128.
		if(iNodeCnt > 128) iNodeCnt = 128;
	  //sanity check.
		if(iNodeCnt < 0) iNodeCnt = 0;
	}
	~MasterBlock()
		{return;}
};

using master_block_t = MasterBlock*;

int writeMasterBlock(BlockDevice *bd, MasterBlock *mb, disk_addr_t disk_location);
int readMasterBlock(BlockDevice *bd, MasterBlock *mb, disk_addr_t disk_location);
void freeMasterBlock(master_block_t mb);
master_block_t allocMasterBlock(int bytes_per_block, int number_of_blocks, disk_addr_t block_map_address);