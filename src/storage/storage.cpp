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
#include "meanbot/mempack/storage/storage.hpp"


namespace meanbot::mempack::storage
{

result<void> storage::init(extalloc *ext_alloc, size_type initial_size/* = config::INITIAL_SIZE_DEFAULT*/, 
	                  size_type max_size/* = config::MAX_SIZE_DEFAULT*/, const config &cfg/* = config()*/)
{
	MEANBOT_RESULT_REQUIRE(initial_size <= config::MAX_SIZE, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(max_size <= config::MAX_SIZE, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(initial_size <= max_size, error::invalid_arg);

	MEANBOT_RESULT_REQUIRE(cfg.segments_initial_count_ >= config::SEGMENTS_MIN_COUNT, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(cfg.segments_initial_count_ >= config::SEGMENTS_MIN_COUNT, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(cfg.segments_initial_count_ <= config::SEGMENTS_MAX_COUNT, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(cfg.segments_max_count_ >= config::SEGMENTS_MIN_COUNT, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(cfg.segments_max_count_ <= config::SEGMENTS_MAX_COUNT, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(cfg.clusters_count_ >= config::CLUSTERS_MIN_COUNT, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(cfg.clusters_count_ <= config::CLUSTERS_MAX_COUNT, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(cfg.usroffptrs_count_ >= config::USROFFPTRS_MIN_COUNT, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(cfg.usroffptrs_count_ <= config::USROFFPTRS_MAX_COUNT, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(cfg.extdata_size_ >= config::EXTDATA_MIN_SIZE, error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(cfg.extdata_size_ <= config::EXTDATA_MAX_SIZE, error::invalid_arg);

	size_type   size          = 0;
	
	size += sizeof(header);

	size_type   segments_size = 0;

	if (auto res = unused_segments::get_mem_size(cfg.segments_initial_count_); !res)
	{
		return fail(res.error());
	}
	else
	{
		segments_size = res.value();
		MEANBOT_RESULT_REQUIRE(detail::checked_ops::add(segments_size, segments_size, sizeof(segment), config::MAX_SIZE), 
		                       error::invalid_arg);
		MEANBOT_RESULT_REQUIRE(detail::checked_ops::add(size, size, segments_size, config::MAX_SIZE), 
		                       error::invalid_arg);
	}

	size_type   clusters_size = 0;

	if (auto res = clusters::get_mem_size(cfg.clusters_count_); !res)
	{
		return fail(res.error());
	}
	else
	{
		clusters_size = res.value();
		MEANBOT_RESULT_REQUIRE(detail::checked_ops::add(clusters_size, clusters_size, sizeof(segment), config::MAX_SIZE), 
		                       error::invalid_arg);
		MEANBOT_RESULT_REQUIRE(detail::checked_ops::add(size, size, clusters_size, config::MAX_SIZE), 
		                       error::invalid_arg);
	}

	size_type   usroffptrs_size = cfg.usroffptrs_count_ * sizeof(offptr_type);

	MEANBOT_RESULT_REQUIRE(detail::checked_ops::add(usroffptrs_size, usroffptrs_size, sizeof(segment), 
	                                                config::MAX_SIZE), error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(detail::checked_ops::add(size, size, usroffptrs_size, config::MAX_SIZE), error::invalid_arg);

	size_type   extdata_size = cfg.extdata_size_;

	MEANBOT_RESULT_REQUIRE(detail::checked_ops::add(extdata_size, extdata_size, sizeof(segment), 
	                                                config::MAX_SIZE), error::invalid_arg);
	MEANBOT_RESULT_REQUIRE(detail::checked_ops::add(size, size, extdata_size, config::MAX_SIZE), error::invalid_arg);

	MEANBOT_RESULT_REQUIRE(detail::checked_ops::add(size, size, segment::MIN_SIZE, config::MAX_SIZE), 
	                       error::invalid_arg);

	MEANBOT_RESULT_REQUIRE(size <= max_size, error::out_of_bounds);
	if (initial_size > size)
	{
		size = initial_size;
	}

	size = detail::byte_alignment::align(size, config::ALIGNMENT);

	MEANBOT_RESULT_REQUIRE(nullptr != (header_ = (header *)ext_alloc->alloc_(ext_alloc->context_, size, 
	                                                                       config::ALIGNMENT)), error::mem_alloc_failed);

	ext_alloc_ = ext_alloc;
	max_size_  = max_size;
	config_    = cfg;
	size_      = size;

	header_->magic_             = header::MAGIC;
	header_->size_              = size;
	header_->version_           = header::VERSION;
	header_->checksum_          = 0;
	header_->segments_offset_   = sizeof(header);
	header_->segments_size_     = segments_size;
	header_->clusters_offset_   = header_->segments_offset_ + header_->segments_size_;
	header_->clusters_size_     = clusters_size;
	header_->usroffptrs_offset_ = header_->clusters_offset_ + header_->clusters_size_;
	header_->usroffptrs_size_   = usroffptrs_size;
	header_->extdata_offset_    = header_->usroffptrs_offset_ + header_->usroffptrs_size_;
	header_->extdata_size_      = extdata_size;
	header_->reserved0_         = 0;
	header_->reserved1_         = 0;
	header_->reserved2_         = 0;

	size_type   unused_segment_offset = header_->extdata_offset_ + header_->extdata_size_;
	size_type   unused_segment_size   = header_->size_ - unused_segment_offset;

	header_->last_segment_size_ = unused_segment_size;

	auto   seg = (segment *)((uint8_t *)header_ + header_->segments_offset_);

	unused_segments_ = (unused_segments *)((uint8_t *)seg + sizeof(segment));

	seg->set_prev_size_and_used(sizeof(header));
	seg->set_size(header_->segments_size_);

	seg = (segment *)((uint8_t *)header_ + header_->clusters_offset_);
	seg->set_prev_size_and_used(header_->segments_size_);
	seg->set_size(header_->clusters_size_);

	seg = (segment *)((uint8_t *)header_ + header_->usroffptrs_offset_);
	seg->set_prev_size_and_used(header_->clusters_size_);
	seg->set_size(header_->usroffptrs_size_);

	seg = (segment *)((uint8_t *)header_ + header_->extdata_offset_);
	seg->set_prev_size_and_used(header_->usroffptrs_size_);
	seg->set_size(header_->extdata_size_);

	seg = (segment *)((uint8_t *)header_ + unused_segment_offset);
	seg->set_prev_size_and_unused(header_->extdata_size_);
	seg->set_size(unused_segment_size);

	auto   unused_segs = (unused_segments *)((uint8_t *)header_ + header_->segments_offset_ + sizeof(segment));

	if (auto res = unused_segs->init(cfg.segments_initial_count_); !res)
	{
		(void) uninit();

		return fail(res.error());
	}
	if (auto res = unused_segs->init(cfg.segments_initial_count_); !res)
	{
		(void) uninit();

		return fail(res.error());
	}
	if (auto res = unused_segs->add(unused_segment_size, unused_segment_offset); !res)
	{
		(void) uninit();

		return fail(res.error());
	}

	return success();
}

result<void> storage::uninit()
{
	return fail(error::not_implemented);
}

bool storage::is_ok() const
{
	return (nullptr != header_);
}

const segment *storage::get_first_segment() const
{
	if (!is_ok())
	{
		return nullptr;
	}

	return (const segment *)((const uint8_t *)header_ + sizeof(header));
}

segment *storage::get_first_segment()
{
	if (!is_ok())
	{
		return nullptr;
	}

	return (segment *)((uint8_t *)header_ + sizeof(header));
}

const segment *storage::get_last_segment() const
{
	if (!is_ok())
	{
		return nullptr;
	}

	return (const segment *)((const uint8_t *)header_ + header_->size_ - header_->last_segment_size_);
}

segment *storage::get_last_segment()
{
	if (!is_ok())
	{
		return nullptr;
	}

	return (segment *)((uint8_t *)header_ + header_->size_ - header_->last_segment_size_);
}

}//namespace meanbot::mempack::storage
