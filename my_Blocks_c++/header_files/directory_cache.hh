#pragma once

#include "inode.hh"
#include "block_device.hh"
#include "blockMap.h"
#include "master_block.hh"

#define MAX_DIR_ENTRY_LEN 256
using INode_t = iNode*;
// A directory is, logically, a mapping from Name -> INode.
// In Python, this could be a hash table mapping directory names to
// an inode number. In C/C++, we'll just use linked lists of
// DirectoryEntry structures.

typedef struct DirectoryEntry {
    INode_t                    inode_ptr;          // this inode
    char *                     name;               // the name for this inode in this directory
    struct DirectoryEntry     *next_sibling;       // siblings in a directory are in a linked list
    struct DirectoryEntry     *maybe_children;     // for directories
    bool                       is_dirty;           // directory needs to be written to disk
} DirectoryEntry, *DirectoryEntry_t, *directory_entry_t;

typedef struct DirectoryEntry *directory_cache_t;

typedef struct FileSystem {
    block_device_t     block_device;
    master_block_t     master_block;
    BlockMap*        block_map;
    iNodeMap*        inode_map;
    directory_cache_t  root_dir;   // this is also our Directory Cache
    // TODO: block_cache_t  block_cache;
} FileSystem, *FileSystem_t, *file_system_t;

// allocates and returns a new DirectoryEntry for an INode
void             addChild(DirectoryEntry_t parent, char *name, INode_t inode);
// if dir is cached, returns its children
// if it is not cached, reads it from its inode, and returns its
// children
DirectoryEntry_t getChildren(DirectoryEntry_t dir, FileSystem_t fs);
// writes a directory (the pairs of name/inode_num of its children) to its inode
void             writeDirectory(DirectoryEntry_t d, FileSystem_t fs);
void             flushDirectoryCache(DirectoryEntry_t dir, FileSystem_t fs);

// utility functions:
// counts # of c's in buf
int countOccurrences(char *buf, char c, int bsize);

// special case directory entry function
void addRoot(FileSystem_t fs, INode_t inode);
