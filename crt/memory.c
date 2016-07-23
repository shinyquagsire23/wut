/****************************************************************************
 * Copyright (C) 2015 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include <malloc.h>
#include <string.h>
#include <coreinit/debug.h>
#include <coreinit/baseheap.h>
#include <coreinit/expandedheap.h>
#include <coreinit/internal.h>

#define MEMORY_ARENA_1          0
#define MEMORY_ARENA_2          1
#define MEMORY_ARENA_3          2
#define MEMORY_ARENA_4          3
#define MEMORY_ARENA_5          4
#define MEMORY_ARENA_6          5
#define MEMORY_ARENA_7          6
#define MEMORY_ARENA_8          7
#define MEMORY_ARENA_FG_BUCKET  8

//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//! Memory functions
//! This is the only place where those are needed so lets keep them more or less private
//!----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

char buf[0x200];

//!-------------------------------------------------------------------------------------------
//! wraps
//!-------------------------------------------------------------------------------------------
void *__wrap_malloc(size_t size)
{
	void *ret = MEMAllocFromExpHeapEx(MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM2), size, 4);
	__os_snprintf(buf, 0x200, "malloc size %x, ret %x", size, ret);
    OSReport(buf);
    return ret;
}

void *__wrap_memalign(size_t align, size_t size)
{
    __os_snprintf(buf, 0x200, "memalign size %x, align %x", size, align);
    OSReport(buf);
    
    if (align < 4)
        align = 4;

    // pointer to a function resolve
    return MEMAllocFromExpHeapEx(MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM2), size, align);
}

void __wrap_free(void *p)
{
    __os_snprintf(buf, 0x200, "free %x", p);
    OSReport(buf);
    
    // pointer to a function resolve
    if(p != 0)
        MEMFreeToExpHeap(MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM2), p);
}

void *__wrap_calloc(size_t n, size_t size)
{
    __os_snprintf(buf, 0x200, "calloc size %x, n %x", size, n);
    OSReport(buf);
    
    void *p = __wrap_malloc(n * size);
	if (p != 0) {
		memset(p, 0, n * size);
	}
	return p;
}

size_t __wrap_malloc_usable_size(void *p)
{
    __os_snprintf(buf, 0x200, "returning malloc usable size");
    OSReport(buf);
    
    //! TODO: this is totally wrong and needs to be addressed
	return 0x7FFFFFFF;
}

void *__wrap_realloc(void *p, size_t size)
{
    __os_snprintf(buf, 0x200, "realloc %x, size %x", p, size);
    OSReport(buf);
    
    void *new_ptr = __wrap_malloc(size);
	if (new_ptr != 0)
	{
		memcpy(new_ptr, p, __wrap_malloc_usable_size(p) < size ? __wrap_malloc_usable_size(p) : size);
		__wrap_free(p);
	}
	return new_ptr;
}

//!-------------------------------------------------------------------------------------------
//! reent versions
//!-------------------------------------------------------------------------------------------
void *__wrap__malloc_r(struct _reent *r, size_t size)
{
    __asm__("mflr 7");
    __os_snprintf(buf, 0x200, "malloc_r size %x, %x", size);
    OSReport(buf);
    
	return __wrap_malloc(size);
}

void *__wrap__calloc_r(struct _reent *r, size_t n, size_t size)
{
    __os_snprintf(buf, 0x200, "calloc_r size %x, n %x", size, n);
    OSReport(buf);

    return __wrap_calloc(n, size);
}

void *__wrap__memalign_r(struct _reent *r, size_t align, size_t size)
{
    __os_snprintf(buf, 0x200, "memalign_r size %x, align %x", size, align);
    OSReport(buf);
    
    return __wrap_memalign(align, size);
}

void __wrap__free_r(struct _reent *r, void *p)
{
    __os_snprintf(buf, 0x200, "free_r %x", p);
    OSReport(buf);
    
    __wrap_free(p);
}

size_t __wrap__malloc_usable_size_r(struct _reent *r, void *p)
{
    __os_snprintf(buf, 0x200, "malloc_usable_size_r");
    OSReport(buf);
    
    return __wrap_malloc_usable_size(p);
}

void *__wrap__realloc_r(struct _reent *r, void *p, size_t size)
{
    __os_snprintf(buf, 0x200, "realloc_r %x, size %x", p, size);
    OSReport(buf);
    
    return __wrap_realloc(p, size);
}
