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
#include "meanbot/mempack/storage/config.hpp"
#include "meanbot/mempack/detail/byte_alignment.hpp"
#include <cstdint>


namespace meanbot::mempack::storage
{

struct clusters_array final
{
	static result<size_type> get_mem_size(size_type cluster_count);
};//clusters_array

}//namespace meanbot::mempack::storage
