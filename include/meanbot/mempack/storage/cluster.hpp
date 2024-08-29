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
#include "meanbot/mempack/detail/config.hpp"
#include "meanbot/mempack/detail/byte_alignment.hpp"
#include "meanbot/mempack/storage/header.hpp"
#include <cstdint>


namespace meanbot::mempack::storage
{

struct cluster final
{
	size_type   block_size_ = 0;
	size_type   bitmap_     = 0;
};//struct cluster final

static_assert(sizeof(cluster) == detail::config::ALIGNMENT);

}//namespace meanbot::mempack::storage
