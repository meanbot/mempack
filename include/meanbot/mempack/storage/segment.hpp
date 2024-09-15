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
#include "meanbot/mempack/size_type.hpp"
#include "meanbot/mempack/storage/config.hpp"
#include "meanbot/mempack/detail/byte_alignment.hpp"
#include "meanbot/mempack/storage/header.hpp"
#include <cstdint>


namespace meanbot::mempack::storage
{

// all size are aligned and at least MIN_SIZE

// storage is divided in segments that are used or free
struct segment final
{
	static constexpr size_type   MIN_SIZE          = config::ALIGNMENT + config::ALIGNMENT;
	static constexpr size_type   MAX_SIZE          = config::MAX_SIZE - sizeof(header);

	static constexpr size_type   STATE_BITMASK     = 1ULL << 63;
	static constexpr size_type   PREV_SIZE_BITMASK = ~STATE_BITMASK;

	static constexpr uint32_t    MERGE_PREV        = 0x01;
	static constexpr uint32_t    MERGE_NEXT        = 0x02;

	size_type   size_                = 0;
	size_type   prev_size_and_state_ = 0;

	size_type get_size() const;

	void set_size(size_type size);

	size_type get_prev_size() const;

	void set_prev_size(size_type prev_size);

	bool is_used() const;

	void set_used();

	bool is_unused() const;

	void set_unused();

	void set_prev_size_and_used(size_type prev_size);

	void set_prev_size_and_unused(size_type prev_size);

	bool is_first_segment(const uint8_t *storage_begin) const;

	bool is_last_segment(const uint8_t *storage_end) const;

	result<segment *> get_prev(const uint8_t *storage_begin);

	result<const segment *> get_prev(const uint8_t *storage_begin) const;

	result<segment *> get_next(const uint8_t *storage_end);

	result<const segment *> get_next(const uint8_t *storage_end) const;

	// split an unused segment B into B1, B2 (B will became B1 and will be set to used and B2 will be set to unused and 
	// will be returned)
	result<segment *> split_unused_segment(size_type size, const uint8_t *storage_end);

	static result<> merge(segment *seg, segment *next_segment, const uint8_t *storage_end);

	// expand the segment into prev and/or next segment if those segments are unused and set it as unused; 
	// return resulting segment
	result<segment *> expand_segment(const uint8_t *storage_begin, const uint8_t *storage_end, 
	                                        uint32_t *merge_info = nullptr);
};//struct segment final

static_assert(sizeof(segment) == config::ALIGNMENT);

}//namespace meanbot::mempack::storage

