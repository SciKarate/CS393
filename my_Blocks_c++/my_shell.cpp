#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <vector>

#include <readline/readline.h>
#include <readline/history.h>
#include "header_files/directory_cache.hh"
#include "header_files/blockMap.h"
#include "header_files/master_block.hh"
#include "header_files/block_device.hh"
#include "header_files/inode.hh"

#define MBLOC 0
#define MAPADDR 1

std::vector<int> grabBlock(std::string deviceName)
{
	MasterBlock* mb;
	char* readbuff[sizeof(MasterBlock)];
	int deviceHandle = open(deviceName.c_str(), O_RDWR | O_CREAT, 0644);
    int numRead = read(deviceHandle, readbuff, sizeof(MasterBlock));
    mb = reinterpret_cast<MasterBlock*>(readbuff);
    std::vector<int> vals = {mb->blockCount, mb->bytesPerBlock};
    return vals;
}

int main(int argc, char** argv) {
  std::cout << "Welcome! You can exit by pressing Ctrl+C at any time...\n";
  std::cout << "Type 'help' to view a list of commands.\n";

  if (argc > 1 && std::string(argv[1]) == "-d")
  {
    // By default readline does filename completion. With -d, we disable this
    // by asking readline to just insert the TAB character itself.
    rl_bind_key('\t', rl_insert);
  }

  char* buf;
  char* splitted;
  char* name;
  int bpb;
  int bcnt;
  int ipb = 0;
  bool mounted = 0;
  bool newly = 0;
  BlockDevice* bdv = NULL;
  MasterBlock* mbl = NULL;
  BlockMap* bmp = NULL;
  iNodeMap* imp = NULL;
  while ((buf = readline(">> ")) != nullptr) {
    if (strlen(buf) > 0) {
      add_history(buf);
    }

    splitted = strtok(buf," ,.-");
    if(!strcmp(splitted, "blockMap"))
    {
    	if(mounted)
    		{dumpMap(bmp);}
    	else
	    	{std::cout << "need to mount!!\n";}
    }
    else if(!strcmp(splitted, "blockMapFull"))
    {
    	if(mounted)
    		{dumpMapFull(bmp);}
    	else
	    	{std::cout << "need to mount!!\n";}
    }
    else if(!strcmp(splitted, "iNodeMapFull"))
    {
    	if(mounted)
    		{dumpiMapFull(imp, mbl->iNodeCnt);}
    	else
	    	{std::cout << "need to mount!!\n";}
    }
    else if (!strcmp(splitted, "newfs"))
    {
    	if(mounted == 1)
    	{
    		std::cout << "You already have a filesystem mounted.\n";
    		std::cout << "... But I won't stop you.\n";
    	}
    	splitted = strtok(NULL, " ,-");
    	name = splitted;
    	splitted = strtok(NULL, " ,-");
    	bpb = std::stoi(splitted);
    	splitted = strtok(NULL, " ,-");
    	bcnt = std::stoi(splitted);
    	remove(name);
    	//std::cout << "newfs " << name << " " << bpb << " " << bcnt;
      //create pointers
    	bdv = new BlockDevice(name, bpb, bcnt);
    	bmp = new BlockMap(bcnt);
    	mbl = allocMasterBlock(bpb, bcnt, MAPADDR);
    	imp = allocateiNodeMap(mbl->iNodeCnt);
      //alloc block
    	setBit(bmp, mbl->diskAddress_iNodeMap);
    	setBit(bmp, mbl->diskAddress);
    	setBit(bmp, 0);
      //init iNodes
    	for(int i = 0; i < mbl->iNodeCnt; i++)
    	{
    		imp->my_nodes[i].inode_num = i;
    		imp->my_nodes[i].cdate = time (NULL);
    		imp->my_nodes[i].mdate = time (NULL);
    		imp->my_nodes[i].num_bytes = 0;
    		imp->my_nodes[i].owner_uid = 0xdeedfeed;
    	}
    	//dumpiMapFull(imp, mbl->iNodeCnt);
      //write data
    	writeMasterBlock(bdv, mbl, MBLOC);
    	writeBlockMap(bdv, bmp, mbl->diskAddress);
    	writeiNodeMap(bdv, imp, mbl->diskAddress_iNodeMap, mbl->iNodeCnt);
      //clear data
    	freeiNodeMap(imp);
    	delete bdv;
    	delete bmp;
    	freeMasterBlock(mbl);
    	if(bpb < 15)
    	{
    		std::cout << "You are using a very low number of bytes per block.\n";
    		std::cout << "Avoid using high block counts in this state.\n";
    		std::cout << "You may experience undocumented behavior.\n";
    	}
    }
    else if (!strcmp(splitted, "allocBlock"))
    {
    	if(mounted)
    		{std::cout << allocBlock(bmp) << std::endl;}
    	else
    		{std::cout << "need to mount!!\n";}
    }
    else if (!strcmp(splitted, "freeBlock"))
    {
    	if(mounted)
    	{
    		splitted = strtok(NULL, " ,-");
    		int freeint = std::stoi(splitted);
    		if(freeint >= bmp->sz)
    			{std::cout << "no.\n";}
    		else
    			{freeBlock(bmp, freeint);}
    	}
    	else
    		{std::cout << "need to mount!!\n";}
    }
    else if (!strcmp(splitted, "setBlock"))
    {
    	if(mounted)
    	{
    		splitted = strtok(NULL, " ,-");
    		int freeint = std::stoi(splitted);
    		if(freeint >= bmp->sz)
    			{std::cout << "no.\n";}
    		else
    			{setBit(bmp, freeint);}
    	}
    	else
    		{std::cout << "need to mount!!\n";}
    }
    else if (!strcmp(splitted, "allociNode"))
    {
    	if(mounted)
    		{std::cout << allocateiNode(imp, mbl->iNodeCnt) << std::endl;}
    	else
    		{std::cout << "need to mount!!\n";}
    }
    else if (!strcmp(splitted, "freeiNode"))
    {
    	if(mounted)
    	{
    		splitted = strtok(NULL, " ,-");
    		int freeint = std::stoi(splitted);
    		if(freeint >= mbl->iNodeCnt)
    			{std::cout << "no.\n";}
    		else
    			{freeiNode(imp, freeint);
    			imp->my_nodes[freeint].mdate = time (NULL);}
    	}
    	else
    		{std::cout << "need to mount!!\n";}
    }
    else if (!strcmp(splitted, "setiNode"))
    {
    	if(mounted)
    	{
    		splitted = strtok(NULL, " ,-");
    		int freeint = std::stoi(splitted);
    		if(freeint >= mbl->iNodeCnt)
    			{std::cout << "no.\n";}
    		else
    			{setiNode(imp, freeint);
    			imp->my_nodes[freeint].mdate = time (NULL);}
    	}
    	else
    		{std::cout << "need to mount!!\n";}
    }
    else if (!strcmp(splitted, "getBlockAddr"))
    {
    	if(mounted)
    	{
    		splitted = strtok(NULL, " ,-");
    		int ind = std::stoi(splitted);
    		splitted = strtok(NULL, " ,-");
    		int offset = std::stoi(splitted);
    		splitted = strtok(NULL, " ,-");
    		bool allocy = std::stoi(splitted);
    		std::cout << getDiskAddressOfBlock(addrToInode(imp, ind), offset, allocy, bmp) << std::endl;
    	}
    	else
    		{std::cout << "need to mount!!\n";}
    }
    else if (!strcmp(splitted, "nodeInfo"))
    {
    	if(mounted)
    	{
    		splitted = strtok(NULL, " ,-");
    		int ind = std::stoi(splitted);
    		std::cout << imp->my_nodes[ind].cdate << std::endl;
    		std::cout << imp->my_nodes[ind].mdate << std::endl;
    	}
    }
    else if (!strcmp(splitted, "mount"))
    {
    	splitted = strtok(NULL, " ,-");
    	name = splitted;
      	  //retrieve bpb and bcnt -------- ???
    	std::vector<int> grabbed = grabBlock(name);
    	bcnt = grabbed[0];
    	bpb = grabbed[1];
      	  //create ptrs
    	ipb = bpb/(sizeof(iNode));
    	bdv = new BlockDevice(name, bpb, bcnt);
    	bmp = new BlockMap(bcnt);
    	mbl = allocMasterBlock(bpb, bcnt, MAPADDR);
      	  //read data from storage
    	readMasterBlock(bdv, mbl, MBLOC);
    	readBlockMap(bdv, bmp, mbl->diskAddress);
    	imp = allocateiNodeMap(mbl->iNodeCnt);
    	readiNodeMap(bdv, imp, mbl->diskAddress_iNodeMap, mbl->iNodeCnt);
    	mounted = 1;
    }
    else if (!strcmp(splitted, "unmount"))
    {
    	if(mounted)
    	{
    		writeBlockMap(bdv, bmp, mbl->diskAddress);
    		writeiNodeMap(bdv, imp, mbl->diskAddress_iNodeMap, mbl->iNodeCnt);
    		delete bdv;
    		delete bmp;
    		freeiNodeMap(imp);
    		freeMasterBlock(mbl);
    		mounted = 0;
    	}
    	else
    		{std::cout << "need to mount!!\n";}
    }
    else if (!strcmp(splitted, "help"))
    {
    	std::cout << "New filesystem:\t\tnewfs <name> <bytesPerBlock> <blockCount>\n";
    	std::cout << "Mount filesystem:\tmount <name>\n";
    	std::cout << "Unmount filesystem:\tunmount <name>\n";
    	std::cout << "Show block map (trnc):\tblockMap\n";
    	std::cout << "Show full block map:\tblockMapFull\n";
    	std::cout << "Show full iNode map:\tiNodeMapFull\n";
    	std::cout << "Alloc first block:\tallocBlock\n";
    	std::cout << "Set block at index:\tsetBlock <index>\n";
    	std::cout << "Free block at index:\tfreeBlock <index>\n";
    	std::cout << "Alloc first iNode:\tallociNode\n";
    	std::cout << "Set iNode at index:\tsetiNode <index>\n";
    	std::cout << "Free iNode at index:\tfreeiNode <index>\n";
    	std::cout << "Get block address:\tgetBlockAddr <index> <block_offset> <alloc_if_free>\n";
    	std::cout << "Get node info:\t\tnodeInfo <index>\n";
    	std::cout << "Quit program:\t\tquit\n";
    }
    else if (!strcmp(splitted, "quit"))
    	{
    		if(mounted != 0)
    		{
    			std::cout << "You forgot to unmount...\n";
    			std::cout << "Hope that's not a problem.\n";
    		}
    		std::cout << "Bye!\n";
    		return 0;
    	}
    else
    	{std::cout << "unknown command: " << buf << std::endl;}

    // readline malloc's a new buffer every time.
    free(buf);
  }

  return 0;
}