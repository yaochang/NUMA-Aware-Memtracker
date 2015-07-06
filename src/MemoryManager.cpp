/*
 * MemoryManager.cpp
 *
 *  Created on: Jul 6, 2015
 *      Author: yaochang
 */




#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "constants.h"
#include "MemoryManager.h"
#include "SpinLock.h"
#include <numa.h>


Memory::Arena::Arena(uint32_t cpu_id, size_t sz, bool isnuma)
{
	//this->memory_manager = mem;
	this->_cpu_id = cpu_id;
	this->_arena_size = sz;
	this->_isnuma = isnuma;
	this->_lock = NULL; 
	if(this->_isnuma){
		this->_numa_node_id = numa_node_of_cpu(cpu_id);
		this->_data = (char*)numa_alloc_onnode(this->_arena_size, this->_numa_node_id);
		this->_cur_ponter = this->_data;
	}else{
		this->_numa_node_id = 0;
		this->_data = (char*) malloc(this->_arena_size);
		this->_cur_ponter = this->_data;
	}
	//initialize the allocated memory
	memset(this->_data, 0, this->_arena_size);
	printf("Arena %d finish initialization\n", cpu_id);
}


bool Memory::Arena::hasAvailableSpace(size_t size)
{
	if( (this->_cur_ponter + size) - this->_data < this->_arena_size){
		return true;
	}else{
		return false;
	}
}


void* Memory::Arena::arena_malloc(size_t size)
{
	void* ret = NULL;
	if(hasAvailableSpace(size)){
		ret = this->_cur_ponter;
		this->_cur_ponter = this->_cur_ponter + size;
	}
	return ret;
}

void Memory::Arena::init_lock()
{
	this->_lock = NULL;
	if(hasAvailableSpace(sizeof(Spinlock))){
		this->_lock = (Spinlock*) arena_malloc(sizeof(Spinlock));
		this->_lock->value = 0;
		this->_lock->yield = true;
	}
	return;
}



void Memory::Arena::arena_free(void *pointer)
{
	/*
	 * Do nothing
	 * */
	return;
}

void Memory::Arena::arena_extend()
{
	/*
	 * Do nothing
	 * */
	return;
}

Memory::Memory::Memory()
{
	if(numa_available() != -1){ //support numa
		this->_isnuma = true;
		this->_numa_node_cnt = numa_num_configured_nodes();
		this->_cpu_cnt = numa_num_configured_cpus();
	}else{
		this->_isnuma = false;
		this->_numa_node_cnt = 0;
		this->_cpu_cnt = 0;
	}
	this->_arena = NULL;
	this->_arena_cnt = 0;
	this->_arena_size = 0;
}

void Memory::Memory::init(uint32_t arena_cnt, uint32_t *cpus)
{
	this->_arena_cnt = arena_cnt;
	this->_arena = new Arena*[this->_arena_cnt];
	size_t arena_size = OneG;
	for(uint32_t i=0;i<this->_arena_cnt;i++){
		this->_arena[i] = new Arena(cpus[i], arena_size, this->_isnuma);
		this->_arena[i]->init_lock();
	}
	printf("Memory finishes initialization\n");
}

bool Memory::Memory::isNumaEnabled()
{
	return this->_isnuma;
}

uint32_t Memory::Memory::getArenaNumber()
{
	return this->_arena_cnt;
}

uint32_t Memory::Memory::getArenaSize()
{
	return this->_arena_size;
}

uint32_t Memory::Memory::getNumaNodeNumber()
{
	return this->_numa_node_cnt;
}

