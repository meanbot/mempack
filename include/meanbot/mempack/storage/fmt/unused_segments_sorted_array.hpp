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


#include "meanbot/mempack/storage/unused_segments_sorted_array.hpp"
#include <fmt/format.h>


template <>
struct fmt::formatter<meanbot::mempack::storage::unused_segments_sorted_array> : fmt::formatter<std::string>
{
	static constexpr auto parse(const format_parse_context &ctx)
	{
		return ctx.begin();
	}

	template <typename format_context>
	auto format(const meanbot::mempack::storage::unused_segments_sorted_array &unused_segments, format_context &ctx)
	{
		auto   out = ctx.out();

		format_to(out, "unused segments ( {} / {} ):\n", unused_segments.used_count_, unused_segments.total_count_);
		format_to(out, "   +-------------+-------------+-------------+\n");
		format_to(out, "   |           # |        SIZE |      OFFSET |\n");
		format_to(out, "   +-------------+-------------+-------------+\n");
		for (meanbot::mempack::size_type i = 0; i < unused_segments.used_count_; i++)
		{
			format_to(out, "   | {:>11d} | {:>11d} | {:>11d} |\n", i, unused_segments.entries_[i].size_, 
			          unused_segments.entries_[i].offset_);
		}
		format_to(out, "   +-------------+-------------+-------------+");

		return out;
	}
};
