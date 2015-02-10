#ifndef __CHECKSUM_H
#define __CHECKSUM_H

#pragma once

#include "util.h"

class checksum;

typedef checksum checksum_str;

class checksum {
private:
	ulong checksum_value;
public:
	checksum();
	checksum(const void *data, int size);
	checksum(const char *string);
	void set(const void *data, int size);
	void set(const char *string);
	ulong get_value() const {return checksum_value;}
	bool invalid();
	void invalidate();

	const bool operator == (const checksum &c) const {
		return (c.get_value() == get_value());
	}
	const bool operator != (const checksum &c) const {
		return (c.get_value() != get_value());
	}
};

class checksum_stri {
private:
	ulong checksum_value;
public:
	checksum_stri();
	checksum_stri(const void *data, int size);
	checksum_stri(const char *string);
	void set(const void *data, int size);
	void set(const char *string);
	void set(ulong val);
	ulong get_value() const {return checksum_value;}
	bool invalid() const;
	void invalidate();

	const bool operator == (const checksum_stri &c) const {
		return (c.get_value() == get_value());
	}
	const bool operator != (const checksum_stri &c) const {
		return (c.get_value() != get_value());
	}
};

#endif //__CHECKSUM_H