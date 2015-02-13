#ifndef __STATIC_POOL_H
#define __STATIC_POOL_H

#pragma once

#include "utlist.h"
#include "../util.h"

// a static_pool is a pool of a specific number objects that are allocated with the pool.
// you can grab objects from it and free objects to it up to the specified limit.  you can
// also use the "used_list" to cycle through all allocated objects.
//
// Static pools can be created with any number of "used lists", allowing you to organize all
// allocated objects into enumerated subsets.
//
// For example, I have used this to allocate UI elements sorted into containers based on their
// render depth.
//

template <class T>
class static_pool {
public:
	T *master_list;
	int num_items;
	int num_free;

	T *free_list;

	T **used_lists;
	int num_used_lists;

	static_pool(int num_to_alloc, int num_lists = 1)
	{
		if (num_to_alloc == 0) {
			num_used_lists = 0;
			num_items = 0;
			num_free = 0;
			master_list = NULL;
			free_list = NULL;
			used_lists = NULL;
			return;
		}

		Assert_return(num_to_alloc > 0);
		Assert_return(num_lists > 0);

		// allocate the items
		master_list = new T[num_to_alloc];
		num_items = num_to_alloc;
		num_free = num_to_alloc;

		// clear both lists
		free_list = NULL;

		num_used_lists = num_lists;
		used_lists = new T*[num_lists];
		for (int i = 0; i < num_lists; i++) {
			used_lists[i] = NULL;
		}

		// populate the free list
		for (int i = 0; i < num_items; i++) {
			DL_APPEND(free_list, &master_list[i]);
		}
	}
	~static_pool()
	{
		if (master_list) {
			delete[] master_list;
		}
		
		if (used_lists) {
			delete[] used_lists;
		}
	}
	T *alloc(int used_list_num = 0);
	void free(T *to_free, int list_num = 0)
	{
		Assert_return(to_free);

		// make sure it's from the correct pool.
		Assert(to_free >= master_list && to_free < (master_list + num_items));
		Assert_return(list_num >= 0 && list_num < num_used_lists);

		Assert_return(used_lists[list_num]);

		DL_DELETE(used_lists[list_num], to_free);

		DL_APPEND(free_list, to_free);
		num_free++;
	}

	// Use the difference in memory locations to find the index within the index.
	int get_index(T *obj) {
		Assert_return_value(obj, -1);
		int mem_diff = (obj - master_list);
		if (mem_diff < 0) {
			return -1;
		}

		// Initially, I thought I had to divide by sizeof(T), but I think the compiler handles that
		// bit for me, so this makes it line up correctly.
		if (mem_diff < num_items) {
			return mem_diff;
		} else {
			return -1;
		}
	}

	int get_num_used()
	{
		return num_items - num_free;
	}
};

template <class T> T *static_pool<T>::alloc(int used_list_num /*= 0*/)
{
	Assert_return_value(used_list_num >= 0 && used_list_num < num_used_lists, NULL);

	// we've reached the limit of this pool.
	if (free_list == NULL) {
		return NULL;
	}
	T *return_val = free_list;

	DL_DELETE(free_list, return_val);
	DL_APPEND(used_lists[used_list_num], return_val);
	num_free--;
	return return_val;
}


#endif //__STATIC_POOL_H