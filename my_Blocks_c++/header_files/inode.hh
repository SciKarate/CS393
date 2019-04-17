#pragma once
#include <string>
#include <time.h>
//#include <vector>
#include "block_device.hh"
#include "blockMap.h"

enum inode_type_t {f, d, s}; // can be either 1 or 2 bytes
using iNodeAddr_t = int16_t;
using disk_address_t = int16_t;
using timestamp_t = time_t;
using block_device_t = BlockDevice*;
#define BLOCK_PTRS_PER_INODE_STRUCT 16 // can be any length, but between 8 and 16 makes the most sense
#define PADDING_PER_INODE 40

class iNode
{
public:
	iNodeAddr_t    inode_num;   // which inode this is (index in the inode map)
	uint32_t       owner_uid;  // who owns this
	uint32_t       gid;        // which group id the perms refer to
	inode_type_t   type;       // file, directory, symlink, ...
	uint16_t       perms;      // RWX permissions for owner / group / all
	uint16_t       status;     // whether it's allocated, needs to be written, etc
	uint16_t       link_count; // how many hard links there are to this iNode
	timestamp_t    cdate;      // creation_time
	timestamp_t    mdate;      // most recent modification time
	uint16_t       level;      // level
	uint64_t       num_bytes;  // the current size of the inode
	disk_address_t block_ptrs[BLOCK_PTRS_PER_INODE_STRUCT]; // data blocks, or blocks of disk_address_t[]
	char padding[PADDING_PER_INODE];      // only include this if you need it, but using padding 
};

class iNodeMap
{
public:
	iNode *my_nodes = NULL;
	//std::vector<iNode> my_nodes;
	iNodeMap(int size)
	{
		my_nodes = (iNode*)calloc(size, sizeof(iNode));
		//my_nodes.resize(size);
	}
	~iNodeMap()
		{return;}
};
using iNodeMap_t = iNodeMap*;

//cout the entire iNodeMap
void dumpiMapFull(iNodeMap* m, int num);
iNodeMap_t allocateiNodeMap(int num_inodes);
int readiNodeMap(BlockDevice *bd, iNodeMap* inode_map, disk_address_t disk_address, int num_inodes);
int writeiNodeMap(BlockDevice *bd, iNodeMap* inode_map, disk_address_t disk_address, int num_inodes);
iNodeAddr_t allocateiNode (iNodeMap_t m, int amnt);
void freeiNode(iNodeMap_t m, iNodeAddr_t inode_to_free);
void setiNode(iNodeMap_t m, iNodeAddr_t inode_to_my_nodes);
iNode* addrToInode(iNodeMap_t m, iNodeAddr_t index);
typedef int block_offmy_nodes_t ;
disk_addr_t getDiskAddressOfBlockRecursive(iNode* inode, block_offmy_nodes_t b, bool alloc_if_absent, int level, BlockMap* block_map);
disk_addr_t getDiskAddressOfBlock(iNode* inode, block_offmy_nodes_t b, bool alloc_if_absent, BlockMap* bm);
void freeiNodeMap(iNodeMap* mb);