#include <iostream>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>
#include "header_files/blockMap.h"
#include "header_files/master_block.hh"
#include "header_files/block_device.hh"
#include "header_files/inode.hh"
typedef iNode* INode_t;
typedef BlockMap* BlockMap_t;
typedef int block_offset_t ;

// the internal, recursive version of gDAOB, but for now only handles
// level 0 INodes
disk_addr_t getDiskAddressOfBlockRecursive(INode_t inode, block_offset_t b, bool alloc_if_absent, int level, BlockMap_t block_map)
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

// getDiskAddressOfBlock - is the main task of an iNode: map file offsets to disk
// addresses. The alloc_with_absent flag used for writes to previously unallocated blocks.
// returns a disk address of the block of data if present or if alloc_if_absent,
// or -1 otherwise
disk_addr_t getDiskAddressOfBlock(INode_t inode, block_offset_t b, bool alloc_if_absent, BlockMap_t bm)
{
    assert(inode && inode->level >= 0);
    return getDiskAddressOfBlockRecursive(inode, b, alloc_if_absent, inode->level, bm);
}

int main()
{
	std::cout << "dang" << std::endl;
}