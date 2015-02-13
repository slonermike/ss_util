#ifndef __fixed_array_H
#define __fixed_array_H

#pragma once

#include "../util.h"

// fixed_array is designed as a storage list with a fixed/predictable memory footprint.

template <class T>
class fixed_array {
	T *m_list_internal;
	int m_size;
	int m_max_size;
public:

	fixed_array(int max_size)
	{
		Assert(max_size >= 1);

		m_size = 0;
		m_max_size = MAX(1, max_size);
		m_list_internal = new T[max_size];
	}
	~fixed_array()
	{
		delete [] m_list_internal;
	}

	// Allow indexing.
	T& operator [] (const long i) {
		Assert(i < m_size);
		return (m_list_internal[i]);
	}
	inline int size() {return m_size;}
	inline int max_size() {return m_max_size;}
	void resize(int new_size);
	void append(const T &new_val);
	void remove(const T &val);
	void remove_at_index(int index);
	void insert_at_index(const T &val, int index);

	T &object_at(int index);
	T *pointer_at(int index);
};

template <class T>
T & fixed_array<T>::object_at( int index )
{
	// The compiler thinks we're returning a local address below, for some reason, but
	// it's actually an address local to the object, rather than the function.  It's a warning,
	// not an error, so I'm gonna let it slide.
	Assert(index < m_size);
	return (T)*(m_list_internal + index);
}

template <class T>
T * fixed_array<T>::pointer_at(int index)
{
	Assert_return_value(index < m_size && index >= 0, NULL);
	return &m_list_internal[index];
}

template <class T>
void fixed_array<T>::remove_at_index( int index )
{
	Assert_return(index >= 0 && index < m_size);
	for (int i = index; i < m_size - 1; i++) {
		m_list_internal[i] = m_list_internal[i+1];
	}
	m_size--;
}

template <class T>
void fixed_array<T>::insert_at_index( const T &val, int index )
{
	Assert_return(m_size < m_max_size);
	Assert_return(index <= m_size);

	for (int i = m_size; i > index; i--)
	{
		m_list_internal[i] = m_list_internal[i-1];
	}
	m_list_internal[index] = val;

	m_size++;
}

template <class T>
void fixed_array<T>::remove( const T &val )
{
	for (int i = 0; i < m_size; i++) {
		if (m_list_internal[i] == val) {
			remove_at_index(i);
		}
	}
}

template <class T>
void fixed_array<T>::append( const T &new_val )
{
	Assert_return(m_size < m_max_size);
	m_list_internal[m_size] = new_val;
	m_size++;
}

template <class T>
void fixed_array<T>::resize( int new_size )
{
	Assert_return(new_size >= 0 && new_size <= m_max_size);
	m_size = new_size;
}


#endif // __fixed_array_H