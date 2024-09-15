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
#include <cstdint>


namespace meanbot::mempack::storage
{

struct config final
{
	static constexpr size_type   ALIGNMENT            = 16;
	static constexpr size_type   MAX_SIZE             = 0x00'00'00'00'00'04'00'00'00'00'00'00ULL; // 1PB

	static constexpr size_type   SEGMENTS_MIN_COUNT   = 64;
	static constexpr size_type   SEGMENTS_MAX_COUNT   = 67108864; //64M

	static constexpr size_type   CLUSTERS_MIN_COUNT   = 2;
	static constexpr size_type   CLUSTERS_MAX_COUNT   = 65536;

	static constexpr size_type   USROFFPTRS_MIN_COUNT = 2;
	static constexpr size_type   USROFFPTRS_MAX_COUNT = 1024;

	static constexpr size_type   EXTDATA_MIN_SIZE     = 16;
	static constexpr size_type   EXTDATA_MAX_SIZE     = 1073741824; //1GB

	static constexpr size_type   SEGMENTS_DEFAULT_INITIAL_COUNT = 4096;
	static constexpr size_type   SEGMENTS_DEFAULT_MAX_COUNT     = SEGMENTS_MAX_COUNT;

	static constexpr size_type   CLUSTERS_DEFAULT_COUNT         = 256;

	static constexpr size_type   USROFFPTRS_DEFAULT_COUNT       = USROFFPTRS_MIN_COUNT;

	static constexpr size_type   EXTDATA_DEFAULT_SIZE           = EXTDATA_MIN_SIZE;

	static constexpr size_type   INITIAL_SIZE_DEFAULT           = 1048576;
	static constexpr size_type   MAX_SIZE_DEFAULT               = 1073741824;

	size_type   segments_initial_count_ = SEGMENTS_DEFAULT_INITIAL_COUNT;
	size_type   segments_max_count_     = SEGMENTS_DEFAULT_MAX_COUNT;
	size_type   clusters_count_         = CLUSTERS_DEFAULT_COUNT;
	size_type   usroffptrs_count_       = USROFFPTRS_DEFAULT_COUNT;
	size_type   extdata_size_           = EXTDATA_DEFAULT_SIZE;
};

}//namespace meanbot::mempack::storage

