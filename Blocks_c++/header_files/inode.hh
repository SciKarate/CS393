#pragma once
#include <string>
#include <vector>
//#include "block_device.hh"

enum inode_type_t {f, d, s}; // can be either 1 or 2 bytes
using iNodeAddr_t = int16_t;
using disk_address_t = int16_t;
using timestamp_t = int64_t;
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
};
using iNodeMap_t = iNodeMap*;

//cout the entire iNodeMap
void dumpiMapFull(iNodeMap* m, int num)
{
	for (int i = 0; i < num; i++)
		{std::cout << m->my_nodes[i].status;}
	std::cout << std::endl;
	for (int i = 0; i < num; i++)
		{std::cout << m->my_nodes[i].inode_num << " ";}
	std::cout << "\nsize: " << num;
	std::cout << std::endl;
}

// allocates and returns an array of iNode structures num_inodes long (aka an iNode Map)
iNodeMap_t allocateiNodeMap(int num_inodes)
{
	int bytes = num_inodes * sizeof(iNode);
	iNodeMap *n = (iNodeMap*)malloc(bytes + 1);
	iNodeMap* m = new(n) iNodeMap(num_inodes);
	return m;
}

// reads an iNode map on block device bd into inode_map allocated above, at disk_address
int readiNodeMap(BlockDevice *bd, iNodeMap* inode_map, disk_address_t disk_address, int num_inodes)
{
	char* buf = (char*) malloc(num_inodes * sizeof(iNode));
	int ipb = (bd->m_bytesPerBlock/sizeof(iNode));
	int blks = num_inodes / ipb;
	if((num_inodes % ipb) != 0) blks++;

	//blks = 1;

	for(int i = 0; i < blks; i++)
	{
		//std::cout << "hey it's working\n";
		buf = (char*)&(inode_map->my_nodes[i*ipb]);
		bd->readBlock(disk_address + i, buf);
	}
	//inode_map->my_nodes = reinterpret_cast<iNode*>(buf);
	return 0;
}

// writes an iNode map to block device bd from inode_map, at disk_address
int writeiNodeMap(BlockDevice *bd, iNodeMap* inode_map, disk_address_t disk_address, int num_inodes)
{
	//std::cout << "dad\t" << disk_address << std::endl;

	int ipb = (bd->m_bytesPerBlock/sizeof(iNode));
	int blks = num_inodes / ipb;
	if((num_inodes % ipb) != 0) blks++;

	for(int i = 0; i < blks; i++)
	{
		//std::cout << ipb << " writing!!\n";
		char* buf = (char*)&(inode_map->my_nodes[i*ipb]);
		bd->writeBlock(disk_address + i, buf);
	}
	//bd->writeBlock(disk_address, reinterpret_cast <const char*>(inode_map));
	return 0;
}

// allocate an iNode and returns its address (or -1 if none free)
iNodeAddr_t allocateiNode (iNodeMap_t m, int amnt)
{
	for(int i = 0; i < amnt; i++)
	{
		if(m->my_nodes[i].status == 0)
		{
			m->my_nodes[i].status = 1;
			return i;
		}
	}
	return -1; //return the address
}

// free an iNode
void freeiNode(iNodeMap_t m, iNodeAddr_t inode_to_free)
{
	m->my_nodes[inode_to_free].status = 0;
}

void setiNode(iNodeMap_t m, iNodeAddr_t inode_to_my_nodes)
{
	m->my_nodes[inode_to_my_nodes].status = 1;
}

// get an iNode structure/object from the iNode map
iNode* addrToInode(iNodeMap_t m, iNodeAddr_t index)
{
	iNode s = m->my_nodes[index];
	iNode *i = &s;
	return i;
}

// the internal, recursive version of gDAOB, but for now only handles
// level 0 INodes
typedef int block_offmy_nodes_t ;
disk_addr_t getDiskAddressOfBlockRecursive(iNode* inode, block_offmy_nodes_t b, bool alloc_if_absent, int level, BlockMap* block_map)
{
    if (level == 0)
    {
        if (b < BLOCK_PTRS_PER_INODE_STRUCT)
        {
            if (inode->block_ptrs[b] > 0)
            	{return (inode->block_ptrs[b]);}
            else if (alloc_if_absent)
            	{return (allocBlock(block_map));}
            else
            {
                fprintf(stderr, "block device is full!");
                return -1;
            }
        }
        else
        {
            fprintf(stderr, "we don't support INodes that big (yet)\n");
            return -1;
        }
    }
    else
    {
        fprintf(stderr, "we don't support INodes with level > 0 (yet)\n");
        return -1;
    }
}

// getDiskAddressOfBlock - is the main task of an iNode: map file offmy_nodess to disk
// addresses. The alloc_with_absent flag used for writes to previously unallocated blocks.
// returns a disk address of the block of data if present or if alloc_if_absent,
// or -1 otherwise
disk_addr_t getDiskAddressOfBlock(iNode* inode, block_offmy_nodes_t b, bool alloc_if_absent, BlockMap* bm)
{
    assert(inode && inode->level >= 0);
    return getDiskAddressOfBlockRecursive(inode, b, alloc_if_absent, inode->level, bm);
}

void freeiNodeMap(iNodeMap* mb)
{
	free(mb);
}