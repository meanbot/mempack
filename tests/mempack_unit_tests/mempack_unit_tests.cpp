//   _ __ ___   ___ _ __ ___  _ __   __ _  ___| | __  
//  | '_ ` _ \ / _ \ '_ ` _ \| '_ \ / _` |/ __| |/ /  mempack - persistent memory allocator
//  | | | | | |  __/ | | | | | |_) | (_| | (__|   <   -------------------------------------
//  |_| |_| |_|\___|_| |_| |_| .__/ \__,_|\___|_|\_\  https://github.com/meanbot/mempack
//                           |_|                    
// 
// SPDX-FileCopyrightText: 2024 meanbot <https://github.com/meanbot>
// SPDX-License-Identifier: MIT

#include <fmt/core.h>
#include <catch2/catch_all.hpp>
#include <meanbot/mempack/storage/header.hpp>
#include <meanbot/mempack/storage/segment.hpp>
#include <meanbot/mempack/storage/segments.hpp>
#include <meanbot/mempack/storage/fmt/header.hpp>
#include <meanbot/mempack/storage/fmt/segments.hpp>
#include <vector>


using storage_mem_type = std::vector<uint8_t>;

struct entry final
{
	meanbot::mempack::size_type   offset_ = 0;
	meanbot::mempack::size_type   size_   = 0;

	using vector = std::vector<entry>;
};//struct entry final


void compare(const meanbot::mempack::storage::segments &s1, const entry::vector &s2)
{
	CATCH_REQUIRE(s1.used_count_ == s2.size());
	for (size_t i = 0; i < s1.used_count_; i++)
	{
		CATCH_REQUIRE(s1.entries_[i].offset_ == s2[i].offset_);
		CATCH_REQUIRE(s1.entries_[i].size_ == s2[i].size_);
	}
}


CATCH_TEST_CASE("segments_container", "")
{
	static const size_t   TOTAL_SEGMENTS_COUNT = 65536;

	static const entry::vector   data = { { 16, 16 }, { 256, 32 }, { 512, 32 }, { 768, 128 }, { 64, 208}, { 48, 256} };

	storage_mem_type   storage_mem(meanbot::mempack::storage::segments::get_mem_size(TOTAL_SEGMENTS_COUNT).value(), 
	                               0xCC);
	auto               storage_mem_size  = storage_mem.size();
	auto               storage_mem_begin = storage_mem.data();
	auto               storage_mem_end   = storage_mem.data() + storage_mem_size;
	auto               segments          = (meanbot::mempack::storage::segments *)storage_mem_begin;
	uint64_t           found_size        = 0;
	uint64_t           found_offset      = 0;
	uint64_t           found_index       = 0;

	CATCH_REQUIRE(segments->init(TOTAL_SEGMENTS_COUNT));

	//add
	CATCH_REQUIRE(segments->add(128, 768));
	CATCH_REQUIRE(!segments->add(128, 0));
	CATCH_REQUIRE(segments->add(16, 16));
	CATCH_REQUIRE(segments->add(256, 48));
	CATCH_REQUIRE(segments->add(32, 256));
	CATCH_REQUIRE(segments->add(208, 64));
	CATCH_REQUIRE(segments->add(32, 512));

	fmt::print("segments container:\n");
	fmt::print("{}\n", *segments);

	compare(*segments, data);

	//search exact
	CATCH_REQUIRE(segments->search(found_index, 128, 768));
	CATCH_REQUIRE(!segments->search(found_index, 127, 0));
	CATCH_REQUIRE(segments->search(found_index, 16, 16));
	CATCH_REQUIRE(segments->search(found_index, 256, 48));
	CATCH_REQUIRE(segments->search(found_index, 32, 256));
	CATCH_REQUIRE(segments->search(found_index, 208, 64));
	CATCH_REQUIRE(segments->search(found_index, 32, 512));

	//remove at least (= exact)
	CATCH_REQUIRE(segments->remove(128, found_size, found_offset));
	CATCH_REQUIRE(segments->remove(32, found_size, found_offset));
	CATCH_REQUIRE(segments->remove(16, found_size, found_offset));
	CATCH_REQUIRE(segments->remove(256, found_size, found_offset));
	CATCH_REQUIRE(segments->remove(32, found_size, found_offset));
	CATCH_REQUIRE(segments->remove(208, found_size, found_offset));

	// re-add
	CATCH_REQUIRE(segments->add(128, 768));
	CATCH_REQUIRE(!segments->add(128, 768));
	CATCH_REQUIRE(segments->add(16, 16));
	CATCH_REQUIRE(segments->add(256, 48));
	CATCH_REQUIRE(segments->add(32, 256));
	CATCH_REQUIRE(segments->add(208, 64));
	CATCH_REQUIRE(segments->add(32, 512));

	compare(*segments, data);

	//search exact
	CATCH_REQUIRE(segments->search(found_index, 128));
	CATCH_REQUIRE(!segments->search(found_index, 1000));
	CATCH_REQUIRE(segments->search(found_index, 16));
	CATCH_REQUIRE(segments->search(found_index, 256));
	CATCH_REQUIRE(segments->search(found_index, 32));
	CATCH_REQUIRE(segments->search(found_index, 208));
	CATCH_REQUIRE(segments->search(found_index, 32));

	//remove exact
	CATCH_REQUIRE(segments->remove(128, 768));
	CATCH_REQUIRE(segments->remove(32, 256));
	CATCH_REQUIRE(segments->remove(16, 16));
	CATCH_REQUIRE(segments->remove(256, 48));
	CATCH_REQUIRE(segments->remove(32, 512));
	CATCH_REQUIRE(segments->remove(208, 64));

	// re-add
	CATCH_REQUIRE(segments->add(128, 768));
	CATCH_REQUIRE(!segments->add(128, 768));
	CATCH_REQUIRE(segments->add(16, 16));
	CATCH_REQUIRE(segments->add(256, 48));
	CATCH_REQUIRE(segments->add(32, 256));
	CATCH_REQUIRE(segments->add(208, 64));
	CATCH_REQUIRE(segments->add(32, 512));

	compare(*segments, data);

	//remove at least 
	CATCH_REQUIRE(segments->remove(100, found_size, found_offset));  // => 128
	CATCH_REQUIRE(segments->remove(30, found_size, found_offset)); // => 32
	CATCH_REQUIRE(segments->remove(16, found_size, found_offset));  // => 10
	CATCH_REQUIRE(segments->remove(256, found_size, found_offset)); // => 250
	CATCH_REQUIRE(segments->remove(32, found_size, found_offset)); // => 31
	CATCH_REQUIRE(segments->remove(208, found_size, found_offset)); // => 207
}

CATCH_TEST_CASE("segment", "")
{
	storage_mem_type   storage_mem(65536, 0xCC);
	auto               storage_mem_size  = storage_mem.size();
	auto               storage_mem_begin = storage_mem.data();
	auto               storage_mem_end   = storage_mem.data() + storage_mem_size;
	auto               header            = (meanbot::mempack::storage::header *)storage_mem_begin;
	auto               segment           = (meanbot::mempack::storage::segment *)(header + 1);

	header->magic_             = meanbot::mempack::storage::header::MAGIC; // = MAGIC
	header->size_              = storage_mem_size; // full storage size
	header->version_           = meanbot::mempack::storage::header::VERSION; // = VERSION
	header->checksum_          = 0; // not used for now
	header->segments_offset_   = 0; // offset of the segments container
	header->segments_size_     = 0; // size of the segments container
	header->clusters_offset_   = 0; // offset of the clusters container
	header->clusters_size_     = 0; // size of the clusters container
	header->usroffptrs_offset_ = 0; // offset of the user offpptrs container
	header->usroffptrs_size_   = 0; // size of the user offpptrs container
	header->extdata_offset_    = 0; // offset of the extra data container
	header->extdata_size_      = 0; // size of the extra data container
	header->reserved0_         = 0; // not used
	header->reserved1_         = 0; // not used
	header->reserved2_         = 0; // not used
	header->reserved3_         = 0; // not used

	segment->set_prev_size_and_unused(sizeof(meanbot::mempack::storage::header));
	segment->set_size(storage_mem_size - sizeof(meanbot::mempack::storage::header));

	fmt::print("storage:\n");
	fmt::print("{}\n", *header);
}

int main(int argc, char *argv[])
{
	int   result = Catch::Session().run(argc, argv);

	return result;
}
