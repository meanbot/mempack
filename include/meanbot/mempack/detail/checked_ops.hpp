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
#include <cstdint>


namespace meanbot::mempack::detail
{

struct checked_ops final
{
	// a is <= max
	template <typename R, typename A, typename B>
	static bool add(R &r, A a, B b, R max)
	{
		if (b > max)
		{
			return false;
		}
		if (max - b < a)
		{
			return false;
		}

		r = a + b;

		return true;
	};
};

}//namespace meanbot::mempack::storage

