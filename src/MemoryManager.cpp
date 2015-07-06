/*
 * MemoryManager.cpp
 *
 *  Created on: Jul 6, 2015
 *      Author: yaochang
 */



#include <numa.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "MemoryManager.h"

namespace Memory
{

Arena::Arena(Memory *mem, uint32_t cpu_id, size_t sz)
{
	this->memory_manager = mem;
	this->_cpu_id = cpu_id;
	this->_arena_size = sz;
	if(memory_manager->isNumaEnabled()){
		this->_numa_node_id = numa_node_of_cpu(cpu_id);
		this->_data = numa_alloc_onnode(this->_arena_size, this->_numa_node_id);
		this->_cur_ponter = this->_data;
	}else{
		this->_numa_node_id = 0;
		this->_data = malloc(this->_arena_size);
		this->_cur_ponter = this->_data;
	}
}

bool Arena::hasAvailableSpace(size_t size)
{
	if( (this->_cur_ponter + size) - this->_data < this->_arena_size){
		return true;
	}else{
		return false;
	}
}


void* Arena::arena_malloc(size_t size)
{
	void* ret = NULL;
	if(hasAvailableSpace(size)){
		ret = this->_cur_ponter;
		this->_cur_ponter = this->_cur_ponter + size;
	}
	return ret;
}

void Arena::arena_free(void *pointer)
{
	/*
	 * Do nothing
	 * */
	return;
}

void Arena::arena_extend()
{
	/*
	 * Do nothing
	 * */
	return;
}

Memory::Memory()
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

void Memory::init(uint32_t arena_cnt, uint32_t *cpus)
{
	this->_arena_cnt = arena_cnt;
	this->_arena = new Arena*[this->_arena_cnt];
	size_t arena_size = 1000000000;
	for(uint32_t i=0;i<this->_arena_size;i++){
		this->_arena[i] = new Arena(this,cpus[i], arena_size);
	}
}

bool Memory::isNumaEnabled()
{
	return this->_isnuma;
}

uint32_t Memory::getArenaNumber()
{
	return this->_arena_cnt;
}

uint32_t Memory::getArenaSize()
{
	return this->_arena_size;
}

uint32_t Memory::getNumaNodeNumber()
{
	return this->_numa_node_cnt;
}


}//end namespace Memory
