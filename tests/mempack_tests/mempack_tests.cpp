//   _ __ ___   ___ _ __ ___  _ __   __ _  ___| | __  
//  | '_ ` _ \ / _ \ '_ ` _ \| '_ \ / _` |/ __| |/ /  mempack - persistent memory allocator
//  | | | | | |  __/ | | | | | |_) | (_| | (__|   <   -------------------------------------
//  |_| |_| |_|\___|_| |_| |_| .__/ \__,_|\___|_|\_\  https://github.com/meanbot/mempack
//                           |_|                    
// 
// SPDX-FileCopyrightText: 2024 meanbot <https://github.com/meanbot>
// SPDX-License-Identifier: MIT

#include <meanbot/result_fmt.hpp>
#include <meanbot/mempack/extallocs/stdlib_extalloc.hpp>
#include <meanbot/mempack/storage/storage.hpp>
#include <meanbot/mempack/storage/unused_segments.hpp>
#include <meanbot/mempack/storage/fmt/storage.hpp>
#include <fmt/core.h>


using storage_mem_type = std::vector<uint8_t>;


int main(int argc, char* argv[])
{
	{
		meanbot::mempack::storage::storage             storage;
		meanbot::mempack::extallocs::stdlib_extalloc   stdlib_extalloc;

		(void) stdlib_extalloc.init();

		auto   res = storage.init(&stdlib_extalloc.extalloc_);

		fmt::print("storage::init = {}\n", res);

		fmt::print("STORAGE:\n{}\n", storage);

		(void) stdlib_extalloc.uninit();

		int x;
		x = 1;
	}


	static const size_t   TOTAL_SEGMENTS_COUNT = 65536;

	storage_mem_type   storage_mem(meanbot::mempack::storage::unused_segments::get_mem_size(
	                                                                               TOTAL_SEGMENTS_COUNT).value(), 0xCC);
	auto               storage_mem_size  = storage_mem.size();
	auto               storage_mem_begin = storage_mem.data();
	auto               storage_mem_end   = storage_mem.data() + storage_mem_size;
	auto               segments          = (meanbot::mempack::storage::unused_segments *)storage_mem_begin;

	if (auto res = segments->init(TOTAL_SEGMENTS_COUNT); !res)
	{
		fmt::print("init : {}\n", res);

		return -1;
	}
	fmt::print("segments initial :\n{}\n", *segments);

	if (auto res = segments->add(128, 0); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after add 128 :\n{}\n", *segments);

	if (auto res = segments->add(10, 16); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after add 10 :\n{}\n", *segments);

	if (auto res = segments->add(228, 48); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after add 228 :\n{}\n", *segments);

	if (auto res = segments->add(12, 256); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after add 12 :\n{}\n", *segments);

	fmt::print("segments after add 228 :\n{}\n", *segments);

	if (auto res = segments->add(200, 64); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after add 200 :\n{}\n", *segments);

	if (auto res = segments->add(12, 512); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after add 12 (2) :\n{}\n", *segments);

	uint64_t   found_size   = 0;
	uint64_t   found_offset = 0;

	if (auto res = segments->remove(128, found_size, found_offset); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after remove 128 :\n{}\n", *segments);

	if (auto res = segments->remove(12, found_size, found_offset); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after remove 12 :\n{}\n", *segments);

	if (auto res = segments->remove(10, found_size, found_offset); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after remove 10 :\n{}\n", *segments);

	if (auto res = segments->remove(228, found_size, found_offset); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after remove 228 :\n{}\n", *segments);

	if (auto res = segments->remove(12, found_size, found_offset); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after remove 12 :\n{}\n", *segments);

	if (auto res = segments->remove(200, found_size, found_offset); !res)
	{
		fmt::print("add : {}\n", res);

		return res.error();
	}
	fmt::print("segments after remove 200 :\n{}\n", *segments);

	return 0;
}
