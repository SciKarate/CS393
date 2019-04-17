
#include "filesystem.h"
#include "directory_cache.h"

int main(int argc, char **argv) {
    {
        newfs("dctest.dev", 1024, 100, 16);
        FileSystem_t   fs = mount("dctest.dev");
        INodeAddr_t i_num = allocateINode(fs);
        INode_t     inode = getINode(fs, i_num);
        addChild(fs->root_dir, "test1", inode);
        unmount(fs);
    }
    {
        FileSystem_t   fs2 = mount("dctest.dev");
        DirectoryEntry_t d = getChildren(fs2->root_dir, fs2);
        while (d != NULL) {
            printf("%s\t", d->name);
            d = d->next_sibling;
        }
        printf("\n");
        INodeAddr_t i_num = allocateINode(fs2);
        INode_t     inode = getINode(fs2, i_num);
        addChild(fs2->root_dir, "test2", inode);
        unmount(fs2);
    }
}
