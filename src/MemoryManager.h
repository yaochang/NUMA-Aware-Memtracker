/*
 * MemoryManager.h
 *
 *  Created on: Jul 4, 2015
 *      Author: yaochang
 */

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <numa.h>
#include "SpinLock.h"

namespace Memory
{

///*
// * When memory is recycled, it will first saved in the recycle list.
// * Those space can be re-malloc
// * */
//struct RecycleNode
//{
//	void address;
//	size_t size;
//};
//class RecycleList
//{
//public:
//	RecycleList();
//	void addNode(RecycleList *node);
//	void removeNode(RecycleList *node);
//	void findFreeSpace(size_t size);
//private:
//	RecycleNode *RLFirst;
//	RecycleNode *RLPreNode;
//	RecycleNode *RLCurNode;
//	RecycleNode *RLNextNode;
//};



/*
 * Memory Arena
 * */
class Arena
{
public:
	Arena(uint32_t cpu_id, size_t sz, bool _isnuma);
	void init_lock();
	bool hasAvailableSpace(size_t size);
	void* arena_malloc(size_t size);	//malloc on the Arena
	void arena_free(void *pointer);		//free a space on the Arena and add it to the RecycleList
	void arena_extend();				//There are no space left on this Arena, extend it
private:
	uint32_t _arena_size;	//Areana size
	uint32_t _numa_node_id;	//NUMA node which this Arena is located on
	uint32_t _cpu_id;		//used only when each thread is assigned to a distinct Arena
	bool _isnuma;

	Spinlock *_lock;

//	RecycleList *_rlist;	//Recycle list, recollect spaces, which aims to improve memory usage
	char *_data;			//Arena Address
	char *_cur_ponter;
};


/*
 * Memory manager
 * */
class Memory
{
public:
	Memory();
	void init(uint32_t arena_cnt, uint32_t *cpus);
	void malloc_arena();
	bool isNumaEnabled();
	uint32_t getArenaNumber();
	uint32_t getArenaSize();
	uint32_t getNumaNodeNumber();

private:
	Arena **_arena;
	uint32_t _arena_cnt;
	size_t _arena_size;
	//machine info
	bool _isnuma;
	uint32_t _numa_node_cnt;
	uint32_t _cpu_cnt;
};



}//end namespace Memory



#endif /* MEMORYMANAGER_H_ */
