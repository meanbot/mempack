//                                             _    
//   _ __ ___   ___ _ __ ___  _ __   __ _  ___| | __  
//  | '_ ` _ \ / _ \ '_ ` _ \| '_ \ / _` |/ __| |/ /  mempack - persistent memory allocator
//  | | | | | |  __/ | | | | | |_) | (_| | (__|   <   -------------------------------------
//  |_| |_| |_|\___|_| |_| |_| .__/ \__,_|\___|_|\_\  https://github.com/meanbot/mempack
//                           |_|                    
// 
// SPDX-FileCopyrightText: 2024 meanbot <https://github.com/meanbot>
// SPDX-License-Identifier: MIT

#include "precomp.hpp"
#include "meanbot/mempack/storage/clusters_array.hpp"


namespace meanbot::mempack::storage
{

result<size_type> clusters_array::get_mem_size(size_type cluster_count)
{
	return success(cluster_count * sizeof(size_type));
}

}//namespace meanbot::mempack::storage
