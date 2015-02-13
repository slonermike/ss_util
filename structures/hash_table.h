#ifndef __HASH_TABLE_H
#define __HASH_TABLE_H

#include "../checksum.h"
#include "static_pool.h"

#pragma once

template <class T>
struct hash_table_entry {
	T m_value;
	checksum_stri m_key;
	hash_table_entry<T> *prev, *next;
	hash_table_entry()
	{
		prev = next = NULL;
	}
};

template <class T>
class hash_table {
private:
	static_pool< hash_table_entry<T> > *m_pool;

public:
	hash_table(int max_entries, int max_slots)
	{
		m_pool = new static_pool< hash_table_entry<T> >(max_entries, max_slots);
	}

	int get_slot(checksum_stri key)
	{
		return key.get_value() % m_pool->num_used_lists;
	}

	void insert(checksum_stri key, T val)
	{
		Assert_return(!key.invalid());
		int slot = get_slot(key);
		hash_table_entry<T> *entry = m_pool->alloc(slot);
		entry->m_key = key;
		entry->m_value = val;
	}

	void insert(const char *key, T val)
	{
		Assert_return(key != NULL);
		insert(checksum_stri(key), val);
	}

	void remove(checksum_stri key, T *val = NULL)
	{
		int slot = get_slot(key);
		hash_table_entry<T> *cur_entry = NULL, *next_entry = NULL;
		DL_FOREACH_DELETE_SAFE(m_pool->used_lists[slot], cur_entry, next_entry)
		{
			if (cur_entry->m_key == key)
			{
				if (val == NULL || *val == cur_entry->m_value)
				{
					m_pool->free(cur_entry, slot);
					break;
				}
			}
		}
	}

	void remove(const char *key, T *val = NULL)
	{
		Assert_return(key != NULL);
		remove(checksum_stri(key), val);
	}

	T get(checksum_stri key, T default_val)
	{
		Assert_return_value(!key.invalid(), default_val);

		int slot = get_slot(key);
		hash_table_entry<T> *cur_entry = NULL;
		DL_FOREACH(m_pool->used_lists[slot], cur_entry)
		{
			if (cur_entry->m_key == key)
			{
				return cur_entry->m_value;
			}
		}

		return default_val;
	}

	T get(const char *key, T default_val)
	{
		if ( key == NULL )
			return default_val;
		else
			return get(checksum_stri(key), default_val);
	}
};


#endif // __HASH_TABLE_H