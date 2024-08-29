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


namespace meanbot::mempack::detail
{

struct byte_alignment final
{
	//align_to_value must be > 0
	template <typename T>
	static constexpr T align(T value, T align_to_value)
	{
		return (value + align_to_value - 1) & ~(align_to_value - 1);
	}

	//align_to_value must be > 0
	template <typename T>
	static constexpr bool is_aligned(T value, T align_to_value)
	{
		return (0 == (value % align_to_value));
	}

	template <typename T>
	static constexpr bool is_power_of_two(T value)
	{
		return (value != 0) && (0 == (value & (value - 1)));
	}

	
};//struct alignment final

}//namespace meanbot::mempack::detail
