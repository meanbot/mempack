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
#include "meanbot/mempack/storage/config.hpp"
#include "meanbot/mempack/detail/byte_alignment.hpp"
#include <cstdint>


namespace meanbot::mempack::storage
{

struct header final
{
	static constexpr size_type   VERSION_0 = 0;

	static constexpr size_type   MAGIC     = 0x21'4B'43'41'50'4D'45'4D;  // 'MEMPACK!'
	static constexpr size_type   VERSION   = VERSION_0;

	size_type   magic_             = 0; // = MAGIC
	size_type   size_              = 0; // full storage size
	size_type   version_           = 0; // = VERSION
	size_type   checksum_          = 0; // not used for now
	size_type   segments_offset_   = 0; // offset of the segments container
	size_type   segments_size_     = 0; // size of the segments container
	size_type   clusters_offset_   = 0; // offset of the clusters container
	size_type   clusters_size_     = 0; // size of the clusters container
	size_type   usroffptrs_offset_ = 0; // offset of the user offpptrs container
	size_type   usroffptrs_size_   = 0; // size of the user offpptrs container
	size_type   extdata_offset_    = 0; // offset of the extra data container
	size_type   extdata_size_      = 0; // size of the extra data container
	size_type   last_segment_size_ = 0; // size of the last segment
	size_type   reserved0_         = 0; // not used
	size_type   reserved1_         = 0; // not used
	size_type   reserved2_         = 0; // not used
};

static_assert(detail::byte_alignment::is_aligned(sizeof(header), config::ALIGNMENT));

}//namespace meanbot::mempack::storage

