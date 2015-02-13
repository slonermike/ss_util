#include "util.h"

#include "malloc/malloc.h"
#include "string.h"

void mem_swap( void *buf1, void *buf2, int size )
{
	Assert_return(size > 0 && buf1 && buf2);
	void *temp_mem = alloca(size);
	memcpy(temp_mem, buf1, size);
	memcpy(buf1, buf2, size);
	memcpy(buf2, temp_mem, size);
}
