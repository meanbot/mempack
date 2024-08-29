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
#include <cstdint>
#include <cstdlib>


namespace meanbot::mempack::extallocs
{

struct stdlib_extalloc final
{
	extalloc   extalloc_;

	static inline const char *name()
	{
		return "stdlib";
	}

	inline bool init()
	{
		extalloc_.name_    = name();
		extalloc_.context_ = nullptr;
		extalloc_.alloc_   = &stdlib_extalloc::alloc;
		extalloc_.realloc_ = &stdlib_extalloc::realloc;
		extalloc_.free_    = &stdlib_extalloc::free;

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

	static inline void *alloc([[maybe_unused]] void *user_context, size_t size, size_t alignment)
	{
		return _aligned_malloc(size, alignment);
	}

	static inline void *realloc([[maybe_unused]] void *user_context, void *mem_ptr, size_t size, size_t alignment)
	{
		return _aligned_realloc(mem_ptr, size, alignment);
	}

	static inline void free([[maybe_unused]] void *user_context, void *mem_ptr)
	{
		_aligned_free(mem_ptr);
	}
};//struct stdlib_extalloc final

}//namespace meanbot::mempack::extallocs
