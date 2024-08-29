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

struct winheap_extalloc final
{
	extalloc   extalloc_;

	static inline const char *name()
	{
		return "winheap";
	}

	inline bool init()
	{
		extalloc_.name_    = name();
		if (nullptr == (extalloc_.context_ = ::HeapCreate(HEAP_NO_SERIALIZE, 0, 0)))
		{
			return false;
		}

		extalloc_.alloc_   = &winheap_extalloc::alloc;
		extalloc_.realloc_ = &winheap_extalloc::realloc;
		extalloc_.free_    = &winheap_extalloc::free;

		return true;
	}

	inline bool uninit()
	{
		extalloc_.name_    = nullptr;
		if (nullptr != extalloc_.context_)
		{
			::HeapDestroy((HANDLE *)extalloc_.context_);
			extalloc_.context_ = nullptr;
		}
		extalloc_.alloc_   = nullptr;
		extalloc_.realloc_ = nullptr;
		extalloc_.free_    = nullptr;

		return true;
	}

	inline bool is_ok() const
	{
		return (nullptr != extalloc_.context_ && nullptr != extalloc_.alloc_ && nullptr != extalloc_.realloc_ && 
		        nullptr != extalloc_.free_);
	}

	// fix alignment!!!
	static inline void *alloc(void *user_context, size_t size, [[maybe_unused]] size_t alignment)
	{
		return ::HeapAlloc((HANDLE *)user_context, 0, size);
	}

	// fix alignment!!!
	static inline void *realloc(void *user_context, void *mem_ptr, size_t size, [[maybe_unused]] size_t alignment)
	{
		return ::HeapReAlloc((HANDLE *)user_context, 0, mem_ptr, size);
	}

	static inline void free(void *user_context, void *mem_ptr)
	{
		::HeapFree((HANDLE *)user_context, 0, mem_ptr);
	}
};//struct winheap_extalloc final

}//namespace meanbot::mempack::extallocs

#endif//#if defined(WIN32)
