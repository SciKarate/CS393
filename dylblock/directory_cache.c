#include "directory_cache.h"
#include <assert.h>

// Allocate a DirectoryEntry for this name->inode mapping and add it
// to the children of parent.
// This is used for both caching and modifying directories.
void addChild(DirectoryEntry_t parent, char *name, INode_t inode){
    DirectoryEntry* newkid = calloc(1, sizeof(DirectoryEntry));
    newkid->inode_ptr = inode;
    newkid->name = name;
    newkid->maybe_children = NULL;
    newkid->next_sibling = NULL;
    newkid->is_dirty = true;

    if(parent->maybe_children != NULL)
    {
    	DirectoryEntry* curr = parent->maybe_children;
    	while (curr->next_sibling)
    		{curr = curr->next_sibling;}
    	curr->next_sibling = newkid;
    }
    else
    	{parent->maybe_children = newkid;}
}

// When creating the root directory entry, we need to pass the
// FileSystem instead of a parent, since there is no parent.
// Also, this is the one directory entry with no name. Hence the
// special case.
void addRoot(FileSystem_t fs, INode_t inode){
    DirectoryEntry* newroot = calloc(1, sizeof(DirectoryEntry));
    newroot->inode_ptr = inode;
    newroot->name = "\"\"";
    newroot->maybe_children = NULL;
    newroot->next_sibling = NULL;
    newroot->is_dirty = true;
    fs->root_dir = newroot;
}

// gets the linked list of children of a directory.
// if it's already been cached, just returns the head of the list.
// if it hasn't been cached yet, it reads the directory's contents
// from its inodes, and creates directory entries for all of the new
// children. Those directory entries (if they're directories) will
// have NULL maybe_children pointers. If a directory has been cached
// but has no children, it will have at least one directory entry for
// ".", which points at its own inode.
// Note: we add the "." entry to mark that a directory has been
// cached, but we don't have ".." entries, which is a common choice:
// this is because an iNode can have links in multiple directories, so
// there is no single parent - instead, the shell would maintain the
// path to get to a file, and implement ".." operations at that level.
//
// a note on memory ownership: each directory entry owns the memory
// pointed to by its "name" entry. Each Directory owns the
// DirectoryEntry entries of its maybe_children linked list.
DirectoryEntry_t getChildren(DirectoryEntry_t dir, FileSystem_t fs) {
    assert(dir && "getChildren dir == null");
    assert(dir->inode_ptr && "getChildren, inode_ptr == null");
    assert(dir->inode_ptr->type == DirectoryType && "getChildren of a non-directory");

    DirectoryEntry* curr = dir;
    DirectoryEntry* subcurr;
    printf(curr->name);
    printf("\n");
    while(curr->maybe_children!=NULL)
    {
    	curr = curr->maybe_children;
    	printf(curr->name);
    	printf("\n");
    	subcurr = curr;
    	while(subcurr->next_sibling!=NULL)
    	{
    		subcurr = subcurr->next_sibling;
    		printf(subcurr->name);
    		printf("\n");
    	}
    }
    printf("\n");
    // your code here, probably don't return NULL
    return curr;
}

// writes a directory to its inode.
// ignores is_dirty flag, so you should check that before calling
// this, if you care about that kind of thing.
void writeDirectory(DirectoryEntry_t d, FileSystem_t fs) {
    // your code here
}

// recursively writes all of the dirty directory entries back to disk
void flushDirectoryCache(DirectoryEntry_t dir, FileSystem_t fs) {
}

// Helper function for getChildren, in case you need one. 
// returns the number of times 'c' appears in buf
int countOccurrences(char *buf, char c, int bsize){
    int ret = 0;
    for (int i = 0; i < bsize; i++) {
        if (buf[i] == c) ret++;
    }
    return ret;
}
