#include "BitSet.h"
#include "master_block.hh"

MasterBlock::MasterBlock(int bytes_per_block, int number_of_blocks, disk_addr_t block_map_address)
{
	bytesPerBlock = bytes_per_block;
	blockCount = number_of_blocks;
	diskAddress = block_map_address;
}