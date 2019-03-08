#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>
typedef std::string file;

class BitSet
{
  public:
	int sz;
	std::vector<bool> set;
	BitSet(int size)
	{
		set.resize(size, false);
		sz = size;
	}
};

BitSet* allocBitSet(int num_bits)
{
	BitSet *s = new BitSet(num_bits);
	return s;
}

//ones bit at index i
int setBit(BitSet* b, int i)
{
	if(b->set[i] == 0)
		{b->set[i] = 1; return 0;}
	else return -1;
}

//zeroes bit at index i
int clearBit(BitSet* b, int i)
{
	if(b->set[i] == 1)
		{b->set[i] = 0; return 0;}
	else return -1;
}

//returns value at index i
bool getBit(BitSet* b, int i)
{
	if(b->set[i] == 0)
		{return false;}
	else return true;
}

//alloc the first available bit. return index -1 if none.
int allocItem(BitSet* b)
{
	for(int i = 0; i < b->sz; i++)
	{
		if(b->set[i] == 0)
			{setBit(b, i); return i;}
	}
	return -1;
}

//free the bit at index i
void freeItem(BitSet* b, int i)
	{clearBit(b,i);}

//read set from file
void readBitSet(file f, BitSet* b)
{
   //read the file into contents
	std::ifstream in(f);
	std::string contents((std::istreambuf_iterator<char>(in)),
	std::istreambuf_iterator<char>());
  //convert dec to bin; save in set
	std::string ostr;
	for(int sum = std::stoi(contents); sum != 0; sum /= 2)
    	{ostr = (sum % 2 == 0 ?"0":"1") + ostr;}
    for(int i = 0; i < b->sz; i++)
    {
    	b->set[i] = int(ostr[i] - 48);
    }
}

//save set to file
void writeBitSet(file f, BitSet* b)
{
	std::ofstream stream;
	stream.open(f);
	if(!stream){std::cout << "oh no" << std::endl;}
	int sum = 0;
	int mul = 1;
  //convert bin to dec
	for(int i = (b->sz - 1); i >= 0; i--)
	{
		sum += mul * (b->set[i]);
		mul *= 2;
	}
	stream << (sum);
	stream.close();
}