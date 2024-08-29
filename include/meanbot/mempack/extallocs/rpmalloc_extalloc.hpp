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
#include <rpmalloc.h>
#include <cstdint>
#include <cstdlib>


namespace meanbot::mempack::extallocs
{

struct rpmalloc_extalloc final
{
	extalloc   extalloc_;

	static inline const char *name()
	{
		return "rpmalloc";
	}

	static inline void global_initialize()
	{
		(void) rpmalloc_initialize();
	}

	static inline void global_finalize()
	{
		rpmalloc_finalize();
	}

	inline bool init()
	{
		extalloc_.name_    = name();
		if (nullptr == (extalloc_.context_ = rpmalloc_heap_acquire()))
		{
			return false;
		}

		extalloc_.alloc_   = &rpmalloc_extalloc::alloc;
		extalloc_.realloc_ = &rpmalloc_extalloc::realloc;
		extalloc_.free_    = &rpmalloc_extalloc::free;

		return true;
	}

	inline bool uninit()
	{
		extalloc_.name_    = nullptr;
		if (nullptr != extalloc_.context_)
		{
			rpmalloc_heap_release((rpmalloc_heap_t *)extalloc_.context_);
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

	static inline void *alloc(void *user_context, size_t size, size_t alignment)
	{
		return rpmalloc_heap_aligned_alloc((rpmalloc_heap_t *)user_context, alignment, size);
	}

	static inline void *realloc(void *user_context, void *mem_ptr, size_t size, size_t alignment)
	{
		return rpmalloc_heap_aligned_realloc((rpmalloc_heap_t *)user_context, mem_ptr, alignment, size, 0);
	}

	static inline void free(void *user_context, void *mem_ptr)
	{
		rpmalloc_heap_free((rpmalloc_heap_t *)user_context, mem_ptr);
	}
};//struct rpmalloc_extalloc final

}//namespace meanbot::mempack::extallocs
