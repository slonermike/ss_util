#include "checksum.h"

#include <ctype.h>
#include <malloc/malloc.h>
#include <string.h>

#define INVALID_CHECKSUM (0)

// Set the checksum value from a chunk of data.
//
// data: data to be checksumed.
// size: the size of the piece of data.
//
// Note: this hash function is the djb2 hash function taken from
// http://www.cse.yorku.ca/~oz/hash.html
//
void checksum::set(const void *data, int size)
{
	checksum_value = 0;
	Assert_return(data);
	Assert_return(size > 0);

	checksum_value = 5381;
	// step through the bytes and hash em up.
	char *ptr = (char*)data;
	for (int i = 0; i < size; i++, ptr++) {
		checksum_value = ((checksum_value << 5) + checksum_value) + (int)(*ptr);
	}
}

// Set the checksum value from a string.
//
// string: string to use for creating the checksum.
//
void checksum::set(const char *string)
{
	invalidate();
	if (string == NULL) {
		return;
	}
	checksum_value = 0;
	int len = strlen(string);
	int size = len * sizeof(char);
	Assert_return(size > 0);
	set(string, size);
}

// Create a checksum from a chunk of data.
//
// data: data to be checksumed.
// size: the size of the piece of data.
checksum::checksum(const void *data, int size)
{
	set(data, size);
}

checksum::checksum()
{
	checksum_value = INVALID_CHECKSUM;
}

// Create a checksum value from a string.
//
// string: string to use for creating the checksum.
//
checksum::checksum(const char *string)
{
	set(string);
}

// Is this checksum valid?
//
bool checksum::invalid()
{
	return checksum_value == INVALID_CHECKSUM;
}

// Make the checksum invalid.
//
void checksum::invalidate()
{
	checksum_value = INVALID_CHECKSUM;
}


//////////////////////////////////////////////////////////////////////////
// case-insensitive version
//////////////////////////////////////////////////////////////////////////

// Set the checksum_stri value from a chunk of data.
//
// data: data to be checksum_stried.
// size: the size of the piece of data.
//
// Note: this hash function is the djb2 hash function taken from
// http://www.cse.yorku.ca/~oz/hash.html
//
void checksum_stri::set(const void *data, int size)
{
	checksum_value = 0;
	Assert_return(data);
	Assert_return(size > 0);

	checksum_value = 5381;
	// step through the bytes and hash em up.
	char *ptr = (char*)data;
	for (int i = 0; i < size; i++, ptr++) {
		checksum_value = ((checksum_value << 5) + checksum_value) + (int)(*ptr);
	}
}

// Set the checksum_stri value from a string.
//
// string: string to use for creating the checksum_stri.
//
void checksum_stri::set(const char *string)
{
	invalidate();

	if (string == NULL) {
		return;
	}

	checksum_value = 0;
	int len = strlen(string);
	int size = len * sizeof(char);
	if (len == 0) {
		return;
	}

	// allocate a new all-caps string on the stack.
	char *lower_string = (char*)alloca(size);
	for (int i = 0; i < size; i++) {
		lower_string[i] = toupper(string[i]);
	}

	set(lower_string, size);
}

void checksum_stri::set( ulong val )
{
	checksum_value = val;
}

// Create a checksum_stri from a chunk of data.
//
// data: data to be checksum_stried.
// size: the size of the piece of data.
checksum_stri::checksum_stri(const void *data, int size)
{
	set(data, size);
}

checksum_stri::checksum_stri()
{
	checksum_value = INVALID_CHECKSUM;
}

// Create a checksum_stri value from a string.
//
// string: string to use for creating the checksum_stri.
//
checksum_stri::checksum_stri(const char *string)
{
	set(string);
}

// Is this checksum_stri valid?
//
bool checksum_stri::invalid() const
{
	return checksum_value == INVALID_CHECKSUM;
}

// Make the checksum_stri invalid.
//
void checksum_stri::invalidate()
{
	checksum_value = INVALID_CHECKSUM;
}