#pragma once
#include <vector>
#include <string>
//#include "block_device.hh"

enum inode_type_t {f, d, s}; // can be either 1 or 2 bytes
using iNodeAddr_t = int16_t;
using disk_address_t = int16_t;
using timestamp_t = int64_t;
using block_device_t = BlockDevice*;
#define BLOCK_PTRS_PER_INODE_STRUCT 8 // can be any length, but between 8 and 16 makes the most sense
#define PADDING_PER_INODE 0

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
	int sz;
	std::vector<iNode> set;
	iNodeMap(int size)
	{
		set.resize(size);
		sz = size;
	}
};
using iNodeMap_t = iNodeMap*;

// allocates and returns an array of iNode structures num_inodes long (aka an iNode Map)
iNodeMap_t allocateiNodeMap(int num_inodes)
{ 
	iNodeMap *m = new iNodeMap(num_inodes);
	return m;
}

// reads an iNode map on block device bd into inode_map allocated above, at disk_address
int readiNodeMap(BlockDevice *bd, iNodeMap_t inode_map, disk_address_t disk_address, int num_inodes)
{
	char readbuff[bd->m_bytesPerBlock];
	bd->readBlock(disk_address, readbuff);
	inode_map = reinterpret_cast<iNodeMap*>(readbuff);
	return 0;
}

// writes an iNode map to block device bd from inode_map, at disk_address
int writeiNodeMap(BlockDevice *bd, iNodeMap_t inode_map, disk_address_t disk_address, int num_inodes)
{
	bd->writeBlock(disk_address, reinterpret_cast <const char*>(inode_map));
	return 0;
}

// allocate an iNode and returns its address (or -1 if none free)
iNodeAddr_t allocateiNode (iNodeMap_t m)
{
	for(int i = 0; i < m->sz; i++)
	{
		if(m->set[i].status == 0)
		{
			m->set[i].status = 1;
			return i;
		}
	}
	return -1; //return the address
}

// free an iNode
void freeiNode(iNodeMap_t m, iNodeAddr_t inode_to_free)
{
	m->set[inode_to_free].status = 0;
}

void setiNode(iNodeMap_t m, iNodeAddr_t inode_to_set)
{
	m->set[inode_to_set].status = 1;
}

// get an iNode structure/object from the iNode map
iNode* addrToInode(iNodeMap_t m, iNodeAddr_t index)
{
	iNode s = m->set[index];
	iNode *i = &s;
	return i;
}

//cout the entire blockMap. don't truncate.
void dumpiMapFull(iNodeMap* m)
{
	for (int i = 0; i < m->sz; i++)
		{std::cout << m->set[i].status;}
	std::cout << "\nsize: " << m->sz;
	std::cout << std::endl;
}