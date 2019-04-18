#include "directory_cache.h"
#include <assert.h>
#include <string.h>
#include <math.h>

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

    //read the block of memory pointed to by dir->inode
    //this will look, for root with child fruit and siblings meat, pie:
    //	fruit|1\nmeat|2\npie|3\n
    //read inode at address 1 into root's maybe_kids
    //read inode at address 2 into root's maybe_kids' sibling
    //read inode at address 3 into root's maybe kids' sibling's sibling.

    if(dir->maybe_children == NULL)
    {
    	DirectoryEntry* currnode = dir;
    	char* readstring = malloc(fs->master_block->bytes_per_block);	
    	iNodeRead(dir->inode_ptr, 0, fs->master_block->bytes_per_block, readstring, fs);
   		//printf("\n"); printf(readstring); printf("\n");

   		int items = 32;

   		char** brokestring = malloc(fs->master_block->bytes_per_block);
   		breakWords(readstring, brokestring, items, "\n |");
   		
   		for(int i = 0; brokestring[i]; i+=2)
   		{
   			//printf(brokestring[i]); printf(brokestring[i+1]); printf(" ");
			addChild(currnode, brokestring[i], &fs->inode_map[atoi(brokestring[i+1])]);		
    	}
    }

    flushDirectoryCache(dir, fs);
    
    /*DirectoryEntry* curr = dir;
    DirectoryEntry* subcurr;
    printf(curr->name); printf("\\ ");
    while(curr->maybe_children!=NULL)
    {
    	curr = curr->maybe_children;
    	printf(curr->name); printf(" \\ ");
    	subcurr = curr;
    	while(subcurr->next_sibling!=NULL)
    	{
    		subcurr = subcurr->next_sibling;
    		printf(subcurr->name); printf("  ");
    	}
    }
    printf("\n\n");
    */
    if(dir->maybe_children)
    	{return dir->maybe_children;}
    else
    	return NULL;
    return NULL;
}

// writes a directory to its inode.
// ignores is_dirty flag, so you should check that before calling
// this, if you care about that kind of thing.
void writeDirectory(DirectoryEntry_t d, FileSystem_t fs)
{
    if(d->maybe_children != NULL)
    {
    	//fs->master_block->bytes_per_block
    	char* writestring = malloc(fs->master_block->bytes_per_block);
    	DirectoryEntry* curr = d->maybe_children;
    	while(curr != NULL)
    	{
    		char* strboy = malloc(strlen(curr->name)+8);
    		sprintf(strboy, "%s|%d\n", curr->name, curr->inode_ptr->inode_num);
    		strcat(writestring, strboy);
    		free(strboy);
    		curr->is_dirty = false;
    		curr = curr->next_sibling;
    	}
    	iNodeWrite(d->inode_ptr, 0, fs->master_block->bytes_per_block, writestring, fs);
    	char* readstring = malloc(fs->master_block->bytes_per_block);
    	iNodeRead(d->inode_ptr, 0, fs->master_block->bytes_per_block, readstring, fs);
    	//printf(writestring); printf("\n"); printf(readstring);
    }
    d->is_dirty = false;
}

// recursively writes all of the dirty directory entries back to disk
void flushDirectoryCache(DirectoryEntry_t dir, FileSystem_t fs)
{
	if(dir->next_sibling != NULL)
		{flushDirectoryCache(dir->next_sibling, fs);}
	if(dir->is_dirty == true)
		{writeDirectory(dir, fs);}
	if(dir->maybe_children != NULL)
	{
		DirectoryEntry_t curr = dir->maybe_children;
		while(curr != NULL)
		{
			if(curr->is_dirty == true) {writeDirectory(dir, fs); return;}
			curr = curr->next_sibling;
		}
		flushDirectoryCache(dir->maybe_children, fs);
		return;
	}
	return;
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
