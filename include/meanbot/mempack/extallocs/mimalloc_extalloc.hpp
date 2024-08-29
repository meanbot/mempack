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


#include "meanbot/mempack/extalloc.hpp"
#include <mimalloc.h>
#include <cstdint>
#include <cstdlib>


namespace meanbot::mempack::extallocs
{

struct mimalloc_extalloc final
{
	extalloc   extalloc_;

	static inline const char *name()
	{
		return "mimalloc";
	}

	inline bool init()
	{
		extalloc_.name_    = name();
		if (nullptr == (extalloc_.context_ = mi_heap_new()))
		{
			return false;
		}
		extalloc_.alloc_   = &mimalloc_extalloc::alloc;
		extalloc_.realloc_ = &mimalloc_extalloc::realloc;
		extalloc_.free_    = &mimalloc_extalloc::free;

		return true;
	}

	inline bool uninit()
	{
		extalloc_.name_    = nullptr;
		if (nullptr != extalloc_.context_)
		{
			mi_heap_destroy((mi_heap_t *)extalloc_.context_);
			extalloc_.context_ = nullptr;
			mi_collect(false);
			mi_collect(true);
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

	static inline void *alloc(void *user_context, size_t size, size_t alignment)
	{
		return mi_heap_malloc_aligned((mi_heap_t *)user_context, size, alignment);
	}

	static inline void *realloc(void *user_context, void *mem_ptr, size_t size, size_t alignment)
	{
		return mi_heap_realloc_aligned((mi_heap_t *)user_context, mem_ptr, size, alignment);
	}

	static inline void free([[maybe_unused]] void *user_context, void *mem_ptr)
	{
		mi_free(mem_ptr);
	}
};//struct mimalloc_extalloc final

}//namespace meanbot::mempack::extallocs
