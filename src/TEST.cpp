/*
 * test.cpp
 *
 *  Created on: Jul 4, 2015
 *      Author: yaochang
 */




#include "MemTrack.h"
#include <stdio.h>
#include <stdlib.h>
#include <numa.h>
using namespace MemTrack;

#define new	MemStamp(__FILE__, __LINE__) * new

class MEM
{
	int a;
	int b[10];
	char c[20];
};

int main(int argc, char**argv)
{
	for(int i=0;i<10;i++){
		MEM *object = MemTrack::new MEM;
	}
	printf("MEM size:%d\n", sizeof(MEM));
	int *p = MemTrack::new int;
	TrackListMemoryUsage();

	if(numa_available() != -1){
		printf("max possible node = %d\n", numa_max_possible_node());
		printf("num possible node = %d\n", numa_num_possible_nodes());
		printf("num of configured node = %d\n", numa_num_configured_nodes());
		printf("num of configured cpu = %d\n", numa_num_configured_cpus());
		long numa_size = 0;
		numa_node_size(0, &numa_size);
		printf("numa node size = %d\n", numa_size);

	}

	return 0;
}
