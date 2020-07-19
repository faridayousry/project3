#include <iostream>
#include  <iomanip>
#include <cmath>

using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)	// Change for different cache sizes
#define		LINE_SIZE		(32)		// Change for different line sizes (4bytes = 1 word, 8bytes = 2 words,...)

const int numberOfLines = CACHE_SIZE / LINE_SIZE;
/*
int* cache[numberOfLines];
for (int i = 0; i < numberOfLines; i++)
	cache[i] = new int[5];
	*/

int cache[numberOfLines][5];		//cache array; 5 coloumns for: index, V-bit, tag, data, offset	??? 

int compulsary_misses = 0;		//counters for each type of miss
int conflict_misses = 0;
int capacity_misses = 0;

enum cacheResType { MISS = 0, HIT = 1 };

/* The following implements a random number generator */
unsigned int m_w = 0xABABAB55;    /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902;    /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
	m_z = 36119 * (m_z & 65535) + (m_z >> 16);
	m_w = 16000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;  /* 32-bit result */
}

unsigned int memGen2()
{
	static unsigned int addr = 0;
	return (addr++) % (DRAM_SIZE);	//iterator from 0 to DRAM_SIZE-1
}

unsigned int memGen3()
{
	static unsigned int addr = 0;
	return  rand_() % (64 * 1024);
}

unsigned int memGen1()
{
	return rand_() % (DRAM_SIZE);		//random number between 0 to DRAM_SIZE-1
}

unsigned int memGen4()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 4);
}

unsigned int memGen5()
{
	static unsigned int addr = 0;
	return (addr++) % (1024 * 64);
}

unsigned int memGen6()
{
	static unsigned int addr = 0;
	return (addr += 64) % (128 * 1024);
}


// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
	//int tag = (addr / LINE_SIZE) % numberOfLines;
	int shift = log2(LINE_SIZE);	//mask get index & tag (remove offset)
	int index = (addr >> shift) % numberOfLines;	//to remove tag
	shift = shift + log2(numberOfLines);
	int tag = addr >> shift;	//shift to remove the number of bits representing the offset & index
	bool missFlag = 1;

	if (cache[index][2] == tag) {		//if the line tag == address tag  << HIT >>
		cache[index][1] = 1;		//set V-bit = 1 (hit)
		return HIT;
	}
	else		//<< MISS >>
	{
		cache[index][2] = tag;		//insert the tag into cache
		for (int i = 0; i < numberOfLines; i++)		//loop over all blocks/lines to check which type of miss
			if (cache[i][1] == 0)	//if all v-bits ==1; then it is a capacity miss
			{
				missFlag = 0;
				break;		//exit loop as soon as one v-bit = 0
			}


		if (missFlag == 1)	//if all v-bit ==1  -> capacity miss
			capacity_misses++;
		else
			if (cache[index][1] == 1)	//if v-bit was already 1 (previous hit)
				conflict_misses++;
			else
				compulsary_misses++;	//if not due to capacity nor conflict, then it is compulsary

		return MISS;

	}



	// This function accepts the memory address for the memory transaction and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss

	return MISS;
}



#ifdef SIM_FA
// Fully Associative Cache Simulator
cacheResType cacheSimFA(unsigned int addr)
{
	// This function accepts the memory address for the read and 
	// returns whether it caused a cache miss or a cache hit

	// The current implementation assumes there is no cache; so, every transaction is a miss
	return MISS;
}
#endif

char* msg[2] = { (char*)"Miss",(char*)"Hit" };

#define		NO_OF_Iterations	100		// Change to 1,000,000 (number of generated addresses)
int main()
{
	unsigned int hit = 0;
	cacheResType r;

	for (int i = 0; i < numberOfLines; i++)		//initialize all V-bits of cache = 0  (miss)
		cache[i][1] = 0;

	unsigned int addr;
	cout << "Direct Mapped Cache Simulator\n";

	for (int inst = 0; inst < NO_OF_Iterations; inst++)	//generate 1000 000 addresser (for each experiment)
	{
		addr = memGen2();			//get random address from 0 to DRAM_SIZE-1
		r = cacheSimDM(addr);
		if (r == HIT) hit++;
		cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r] << ")\n";
	}
	cout << "Hit ratio = " << (100 * hit / NO_OF_Iterations) << endl;
	cout << "Miss ratio = " << (100 * (NO_OF_Iterations - hit) / NO_OF_Iterations) << endl;
	cout << "Compulsary misses = " << compulsary_misses << endl;
	cout << "Conflict misses = " << conflict_misses << endl;
	cout << "Capacity misses = " << capacity_misses << endl;

}
