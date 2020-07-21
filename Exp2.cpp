#include <iostream>
#include  <iomanip>
#include <cmath>

using namespace std;

#define		DBG				1
#define		DRAM_SIZE		(64*1024*1024)
#define		CACHE_SIZE		(64*1024)	// Change for different cache sizes
#define		LINE_SIZE (16)		//line size fixed at 16 bytes

const int numberOfLines = CACHE_SIZE / LINE_SIZE;

int cache[numberOfLines][2];		//cache array; tag & v-bit


enum cacheResType {MISS=0, HIT=1};

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
	static unsigned int addr=0;
	return (addr++)%(DRAM_SIZE);	//iterator from 0 to DRAM_SIZE-1
}

unsigned int memGen3()
{
	static unsigned int addr=0;		//random addresses between 0 to 64K-1
	return  rand_()%(64*1024);
}

unsigned int memGen1()
{
	return rand_()%(DRAM_SIZE);		//random number between 0 to DRAM_SIZE-1
}

unsigned int memGen4()
{
	static unsigned int addr=0;
	return (addr++)%(1024*4);		//iterator from 0 to 4k-1
}

unsigned int memGen5()
{
	static unsigned int addr=0;
	return (addr++)%(1024*64);		//iterator from 0 to 64k-1
}

unsigned int memGen6()
{
	static unsigned int addr=0;
	return (addr+=64)%(128*1024);		//iterator from 0 to 128k-1  (address 64 bit aligned)
}


// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
	//int tag = (addr / LINE_SIZE) % numberOfLines;
	int shift = log2(LINE_SIZE);	//mask get index & tag (remove offset)
	int index = (addr >> shift) % numberOfLines;	//to remove tag

	shift = shift + log2(numberOfLines);
	int tag = addr >> shift;	//shift to remove the number of bits representing the offset & index

	if ((cache[index][0]==1)&(cache[index][1] == tag)) {		//if the line tag == address tag & v-bit = 1  << HIT >>
		return HIT;
	}
		else		//<< MISS >>
        {
			cache[index][1] = tag;		//insert the tag into cache
			cache[index][0] =   1;		//set v-bit to valid
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

char *msg[2] = {(char *)"Miss",(char *)"Hit"};

#define		NO_OF_Iterations	1000000	// Change to 1,000,000 (number of generated addresses)
int main()
{
unsigned int addr;
cout << "Direct Mapped Cache Simulator\n";
cout<<"For cache size: "<<CACHE_SIZE<<endl;
    for (int g=1;g<=6;g++)
    {
	unsigned int hit =0;
	cacheResType r;

	for (int i = 0; i < numberOfLines; i++)		//initialize all V-bits of cache = 0  (not valid)
		cache[i][0] = 0;

	for(int inst=0;inst<NO_OF_Iterations;inst++)	//generate 1000 000 addresses (for each experiment)
	{
	    switch (g){
		case 1: addr = memGen1();break;
		case 2: addr = memGen2();break;
		case 3: addr = memGen3();break;
		case 4: addr = memGen4();break;
		case 5: addr = memGen5();break;
		case 6: addr = memGen6();break;
		}			//get random address from 0 to DRAM_SIZE-1
		r = cacheSimDM(addr);
		if(r == HIT)
            hit++;

		//cout <<"0x" << setfill('0') << setw(8) << hex << addr <<" ("<< msg[r] <<")\n"; // skipped to handle very high processing time in printing a 1000000 line per experiment
	}
	cout<<" Results for generator "<<g<<" is "<<endl;
	cout << dec<<"Hit ratio = " << (100*(float)hit/(float)NO_OF_Iterations)<< endl;
	cout << "Miss ratio = " << ((100 * (float)(NO_OF_Iterations-hit))/(float) NO_OF_Iterations) << endl;
    }

}
