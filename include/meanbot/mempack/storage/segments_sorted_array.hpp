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


#include <meanbot/error.hpp>
#include <meanbot/result.hpp>
#include "meanbot/mempack/storage/segment.hpp"
#include "meanbot/mempack/detail/config.hpp"
#include "meanbot/mempack/detail/byte_alignment.hpp"
#include <cstdint>


namespace meanbot::mempack::storage
{

struct segments_sorted_array final
{
	struct entry final
	{
		size_type   offset_ = 0;
		size_type   size_   = 0;
	};//struct entry final

	size_type   total_count_ = 0;
	size_type   used_count_  = 0;
	entry       entries_[1];

	static result<size_type> get_mem_size(size_type total_segment_count);

	result<> init(size_type total_segments_count);

	bool search(uint64_t &index, uint64_t size, uint64_t offset = INVALID_SIZE);

	result<> insert_at_index(uint64_t index, uint64_t size, uint64_t offset);

	result<> remove_at_index(uint64_t index);

	result<> add(size_type size, size_type offset);

	// remove an unused exact segment
	result<> remove(size_type size, size_type offset);

	// remove an unused segment that has at least <size> size
	result<> remove(size_type size, uint64_t &found_size, uint64_t &found_offset);
};//segments_sorted_array

}//namespace meanbot::mempack::storage
