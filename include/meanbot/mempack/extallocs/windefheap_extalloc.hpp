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

struct windefheap_extalloc final
{
	extalloc   extalloc_;

	static inline const char *name()
	{
		return "windefheap";
	}

	inline bool init()
	{
		extalloc_.name_    = name();
		extalloc_.context_ = nullptr;
		extalloc_.alloc_   = &windefheap_extalloc::alloc;
		extalloc_.realloc_ = &windefheap_extalloc::realloc;
		extalloc_.free_    = &windefheap_extalloc::free;

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

	// fix alignment!!!
	static inline void *alloc([[maybe_unused]] void *user_context, size_t size, [[maybe_unused]] size_t alignment)
	{
		return ::HeapAlloc(::GetProcessHeap(), 0, size);
	}

	// fix alignment!!!
	static inline void *realloc([[maybe_unused]] void *user_context, void *mem_ptr, size_t size, 
	                            [[maybe_unused]] size_t alignment)
	{
		return ::HeapReAlloc(::GetProcessHeap(), 0, mem_ptr, size);
	}

	static inline void free([[maybe_unused]] void *user_context, void *mem_ptr)
	{
		::HeapFree(::GetProcessHeap(), 0, mem_ptr);
	}
};//struct windefheap_extalloc final

}//namespace meanbot::mempack::extallocs


#endif//#if defined(WIN32)
