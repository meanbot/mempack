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


#include <cstdint>


namespace meanbot::mempack
{

struct extalloc final
{
	using AllocMemFunc   = void * (*)(void *user_context, size_t size, size_t alignment);
	using ReallocMemFunc = void * (*)(void *user_context, void *mem_ptr, size_t size, size_t alignment);
	using FreeMemFunc    = void (*)(void *user_context, void *mem_ptr);

	const char       *name_    = nullptr;
	void             *context_ = nullptr;
	AllocMemFunc     alloc_    = nullptr;
	ReallocMemFunc   realloc_  = nullptr;
	FreeMemFunc      free_     = nullptr;
};//struct extalloc final

}//namespace meanbot::mempack
