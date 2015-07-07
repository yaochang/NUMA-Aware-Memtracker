/*
 * test.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: yaochang
 */





#include <stdio.h>
#include <stdlib.h>
#include <numa.h>

#include "MemTrack.h"
#include "MemoryManager.h"

using namespace MemTrack;
using namespace Memory;

//#define new	MemStamp(__FILE__, __LINE__) * new

class MEM
{
	int a;
	int b[10];
	char c[20];
};



int main(int argc, char**argv)
{
	for(int i=0;i<10;i++){
		MEM *object = new MEM;
	}
	printf("MEM size:%d\n", sizeof(MEM));
	int *p = new int;
	TrackListMemoryUsage();

//	if(numa_available() != -1){
//		printf("max possible node = %d\n", numa_max_possible_node());
//		printf("num possible node = %d\n", numa_num_possible_nodes());
//		printf("num of configured node = %d\n", numa_num_configured_nodes());
//		printf("num of configured cpu = %d\n", numa_num_configured_cpus());
//		long numa_size = 0;
//		numa_node_size(0, &numa_size);
//		printf("numa node size = %d\n", numa_size);
//
//	}


	uint32_t cpus[8];
	for(uint32_t i=0;i<8;i++){
		cpus[i] = i;
	}
	Memory::Memory  *mem = new Memory::Memory();
	mem->init(8,cpus);


	return 0;
}
