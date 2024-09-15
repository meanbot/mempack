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


#include "meanbot/mempack/storage/storage.hpp"
#include "meanbot/mempack/storage/fmt/header.hpp"
#include "meanbot/mempack/storage/fmt/unused_segments.hpp"
#include <fmt/format.h>


template <>
struct fmt::formatter<meanbot::mempack::storage::storage> : fmt::formatter<std::string>
{
	static constexpr auto parse(const format_parse_context &ctx)
	{
		return ctx.begin();
	}

	template <typename format_context>
	auto format(const meanbot::mempack::storage::storage &storage, format_context &ctx)
	{
		auto   out = ctx.out();

		format_to(out, "{}\n", *storage.header_);
		format_to(out, "{}\n", *storage.unused_segments_);

		format_to(out, "segments\n");
		format_to(out, "   +-------------+-------------+-------------+--------+-------------+\n");
		format_to(out, "   |           # |        SIZE |      OFFSET |  STATE |   PREV SIZE |\n");
		format_to(out, "   +-------------+-------------+-------------+--------|-------------+\n");

		auto     seg         = storage.get_first_segment();
		auto     storage_end = (const uint8_t *)storage.header_ + storage.header_->size_;
		size_t   idx         = 0;

		while (nullptr != seg)
		{
			format_to(out, "   | {:>11d} | {:>11d} | {:>11d} | {:^6s} | {:>11d} |\n", idx, 
			          seg->get_size(), (const uint8_t *)seg - (const uint8_t *)storage.header_, 
			          seg->is_used() ? "USED" : "UNUSED", seg->get_prev_size());
			if (auto res = seg->get_next(storage_end); res)
			{
				seg = res.value();
			}
			else
			{
				seg = nullptr;

				break;
			}
			idx++;
		}

		format_to(out, "   +-------------+-------------+-------------+--------|-------------+");

		return out;
	}
};
