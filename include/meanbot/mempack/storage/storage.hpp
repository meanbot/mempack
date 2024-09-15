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


#include "meanbot/mempack/offptr_type.hpp"
#include "meanbot/mempack/size_type.hpp"
#include "meanbot/mempack/detail/byte_alignment.hpp"
#include "meanbot/mempack/storage/config.hpp"
#include "meanbot/mempack/storage/header.hpp"
#include "meanbot/mempack/storage/unused_segments.hpp"
#include "meanbot/mempack/storage/clusters.hpp"
#include "meanbot/mempack/detail/checked_ops.hpp"
#include "meanbot/mempack/extalloc.hpp"
#include <meanbot/result.hpp>
#include <meanbot/result_require.hpp>
#include <cstdint>


namespace meanbot::mempack::storage
{

struct storage final
{
	extalloc          *ext_alloc_ = nullptr;
	config            config_;
	size_type         size_       = 0;
	size_type         max_size_   = 0;
	header            *header_    = nullptr;
	unused_segments   *unused_segments_  = nullptr;

	result<void> init(extalloc *ext_alloc, size_type initial_size = config::INITIAL_SIZE_DEFAULT, 
	                  size_type max_size = config::MAX_SIZE_DEFAULT, const config &cfg = config());

	result<void> uninit();

	bool is_ok() const;

	const segment *get_first_segment() const;

	segment *get_first_segment();

	const segment *get_last_segment() const;

	segment *get_last_segment();
};

}//namespace meanbot::mempack::storage

