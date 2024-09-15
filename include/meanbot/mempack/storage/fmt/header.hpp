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


#include "meanbot/mempack/storage/header.hpp"
#include "meanbot/mempack/storage/segment.hpp"
#include <fmt/format.h>


template <>
struct fmt::formatter<meanbot::mempack::storage::header> : fmt::formatter<std::string>
{
	static constexpr auto parse(const format_parse_context &ctx)
	{
		return ctx.begin();
	}

	template <typename format_context>
	auto format(const meanbot::mempack::storage::header &header, format_context &ctx)
	{
		auto   out = ctx.out();

		std::string_view   magic((const char *)&header.magic_, sizeof(header.magic_));
		auto               segment_eof = (const meanbot::mempack::storage::segment *)
		                                                                        ((const uint8_t *)&header + header.size_);
		auto               segment     = (const meanbot::mempack::storage::segment *)(&header + 1);

		format_to(out, "header:\n");
		format_to(out, "   magic             = {}\n", magic);
		format_to(out, "   size              = {}\n", header.size_);
		format_to(out, "   version           = {}\n", header.version_);
		format_to(out, "   checksum          = {}\n", header.checksum_);
		format_to(out, "   segments_offset   = {}\n", header.segments_offset_);
		format_to(out, "   segments_size     = {}\n", header.segments_size_);
		format_to(out, "   clusters_offset   = {}\n", header.clusters_offset_);
		format_to(out, "   clusters_size     = {}\n", header.clusters_size_);
		format_to(out, "   usroffptrs_offset = {}\n", header.usroffptrs_offset_);
		format_to(out, "   usroffptrs_size   = {}\n", header.usroffptrs_size_);
		format_to(out, "   extdata_offset    = {}\n", header.extdata_offset_);
		format_to(out, "   extdata_size      = {}\n", header.extdata_size_);
		format_to(out, "   last_segment_size = {}\n", header.last_segment_size_);
		format_to(out, "   reserved0         = {}\n", header.reserved0_);
		format_to(out, "   reserved1         = {}\n", header.reserved1_);
		format_to(out, "   reserved2         = {}", header.reserved2_);

		return out;
	}
};
