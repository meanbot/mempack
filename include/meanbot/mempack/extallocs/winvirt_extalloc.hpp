//                                             _    
//   _ __ ___   ___ _ __ ___  _ __   __ _  ___| | __  
//  | '_ ` _ \ / _ \ '_ ` _ \| '_ \ / _` |/ __| |/ /  mempack - persistent memory allocator
//  | | | | | |  __/ | | | | | |_) | (_| | (__|   <   -------------------------------------
//  |_| |_| |_|\___|_| |_| |_| .__/ \__,_|\___|_|\_\  https://github.com/meanbot/mempack
//                           |_|                    
// 
// SPDX-FileCopyrightText: 2024 meanbot <https://github.com/meanbot>
// SPDX-License-Identifier: MIT

#pragma once


#if defined(WIN32)


#include "meanbot/mempack/extalloc.hpp"
#include <cstdint>
#include <cstdlib>
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
	#define NOMINMAX
#endif
#include <Windows.h>


namespace meanbot::mempack::extallocs
{

struct winvirt_extalloc final
{
	extalloc   extalloc_;

	static inline const char *name()
	{
		return "winvirt";
	}

	inline bool init()
	{
		extalloc_.name_    = name();
		extalloc_.context_ = nullptr;
		extalloc_.alloc_   = &winvirt_extalloc::alloc;
		extalloc_.realloc_ = &winvirt_extalloc::realloc;
		extalloc_.free_    = &winvirt_extalloc::free;

		return true;
	}

	inline bool uninit()
	{
		extalloc_.name_    = nullptr;
		extalloc_.context_ = nullptr;
		extalloc_.alloc_   = nullptr;
		extalloc_.realloc_ = nullptr;
		extalloc_.free_    = nullptr;

		return true;
	}

	inline bool is_ok() const
	{
		return (nullptr != extalloc_.alloc_ && nullptr != extalloc_.realloc_ && nullptr != extalloc_.free_);
	}

	// alignment is ok up to 64KB !!!
	static inline void *alloc([[maybe_unused]] void *user_context, size_t size, [[maybe_unused]] size_t alignment)
	{
		size_t   *tmp = (size_t *)::VirtualAlloc(nullptr, sizeof(size_t) + size, MEM_RESERVE | MEM_COMMIT, 
		                                         PAGE_READWRITE);

		if (nullptr == tmp)
		{
			return nullptr;
		}
		else
		{
			*tmp = size;

			return tmp + 1;
		}
	}

	// alignment is ok up to 64KB !!!
	static inline void *realloc([[maybe_unused]] void *user_context, void *mem_ptr, size_t size, 
	                            [[maybe_unused]] size_t alignment)
	{
		if (nullptr == mem_ptr)
		{
			return alloc(user_context, size, alignment);
		}

		auto   new_mem = alloc(user_context, size, alignment);

		if (nullptr == new_mem)
		{
			return nullptr;
		}

		auto   ptr_tmp  = ((size_t *)mem_ptr) - 1;
		auto   old_size = *ptr_tmp;

		if (old_size < size)
		{
			memcpy(new_mem, mem_ptr, old_size);
		}
		else
		{
			memcpy(new_mem, mem_ptr, size);
		}

		free(user_context, mem_ptr);

		return new_mem;
	}

	static inline void free([[maybe_unused]] void *user_context, void *mem_ptr)
	{
		auto   ptr_tmp  = ((size_t *)mem_ptr) - 1;

		::VirtualFree(ptr_tmp, 0, MEM_RELEASE);
	}
};//struct winvirt_extalloc final

}//namespace meanbot::mempack::extallocs


#endif//#if defined(WIN32)
