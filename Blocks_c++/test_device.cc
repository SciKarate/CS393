#include <iostream>
#include <cstring>
#include "header_files/block_device.hh"
#include "header_files/master_block.hh"
#include "header_files/blockMap.h"
#include "header_files/inode.hh"

#define BLOCKSIZE 1024
int main() {
    char buf[BLOCKSIZE] = " hello there shiny block device";
    char readBuf[BLOCKSIZE];
    BlockDevice t = BlockDevice("foo.dev", BLOCKSIZE, 100);
    for (int j = 0; j < 10; j++) {
        buf[j] = '0' + j;
        t.writeBlock(j, buf);
        memset(readBuf, 0, BLOCKSIZE);
        t.readBlock(j, readBuf);
        for (int i = 0; i < BLOCKSIZE; i++){
            if (buf[i] != readBuf[i]) {
                std::cout << "buffer mismatch\n";
            }
        }
    }

    std::cout << "time to test my work!\n";
    BlockDevice *s = &t;

    MasterBlock* n = allocMasterBlock(BLOCKSIZE, 100, 1);
    writeMasterBlock(s, n, 0);
    readMasterBlock(s, n, 0);
    BlockMap mappy = BlockMap(BLOCKSIZE);
    for(int i = 0; i < BLOCKSIZE; i++)
    {
    	if(i%8 == 0)
    	{
    		setBit(&mappy, i);
    		allocBlock(&mappy);
    	}
    }
    freeBlock(&mappy, 0);
    freeBlock(&mappy, 16);
    writeBlockMap(s, &mappy, n->diskAddress);
   	readBlockMap(s, &mappy, n->diskAddress);
    dumpMap(&mappy);

    std::cout << "Let's test iNodes!!\n";
    iNodeMap* i = NULL;
    i = allocateiNodeMap(8);
    allocateiNode(i); allocateiNode(i); setiNode(i,3);
    dumpiMapFull(i);
    writeiNodeMap(s, i, 2, 8);
    delete i;
    i = new iNodeMap(8);
    dumpiMapFull(i);
    readiNodeMap(s, i, 2, 8);
    dumpiMapFull(i);

    t.closeDevice();
    freeMasterBlock(n);
    std::cout << "device tests complete\n";
    std::cout << sizeof(iNode) << std::endl;
}

/*
newfs
	creates block device
		bytes per block
		block count

mount mydisdk.dev
	open file for read in non-block mode to pull of first bytes
*/