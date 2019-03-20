#pragma once
#include "block_device.hh"

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
		iNodeCnt = bytesPerBlock/128;
		diskAddress = block_map_address;
		while(number_of_blocks > 0)
		{
			number_of_blocks -= bytes_per_block;
			protect++;
		}
		diskAddress_iNodeMap = block_map_address + protect;
	}
	~MasterBlock()
		{return;}
};

using master_block_t = MasterBlock*;

// writes master block pointed to by mb to the block device @ disk_location
// returns 0 on success, <0 if the write fails
int writeMasterBlock(BlockDevice *bd, MasterBlock *mb, disk_addr_t disk_location)
{
	bd->writeBlock(disk_location, reinterpret_cast <const char*>(mb));
	return 0;
}

// reads a master block from block device @ disk_location
// filling in the passed-in structure
// returns 0 on success, <0 on failure (e.g., magic doesn't match)
int readMasterBlock(BlockDevice *bd, MasterBlock *mb, disk_addr_t disk_location)
{
	MasterBlock* checkptr = mb;
	char readbuff[bd->m_bytesPerBlock];
	bd->readBlock(disk_location, readbuff);
	mb = reinterpret_cast<MasterBlock*>(readbuff);
	if(mb == checkptr)
	{
		std::cout << "huh. weird.\n";
		return 1;
	}
	return 0;
}

// free memory allocated in allocateMasterBlock
void freeMasterBlock(master_block_t mb)
{
	mb->~MasterBlock();
	free(mb);
}

// allocate memory for a Master Block and initialize its fields
// with the parameters to this function
master_block_t allocMasterBlock(int bytes_per_block, int number_of_blocks, disk_addr_t block_map_address)
{
	//MasterBlock *m = new MasterBlock(bytes_per_block, number_of_blocks, block_map_address);
	MasterBlock *n = (MasterBlock*)malloc(bytes_per_block + 1);
	MasterBlock* m = new(n) MasterBlock(bytes_per_block, number_of_blocks, block_map_address);
	return m;
}