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
#include "meanbot/mempack/storage/segment.hpp"


namespace meanbot::mempack::storage
{

size_type segment::get_size() const
{
	return size_;
}

void segment::set_size(size_type size)
{
	size_ = size;
}

size_type segment::get_prev_size() const
{
	return (prev_size_and_state_ & PREV_SIZE_BITMASK);
}

void segment::set_prev_size(size_type prev_size)
{
	prev_size_and_state_ = (prev_size_and_state_ & STATE_BITMASK) | (prev_size & PREV_SIZE_BITMASK);
}

bool segment::is_used() const
{
	return (STATE_BITMASK == (prev_size_and_state_ & STATE_BITMASK));
}

void segment::set_used()
{
	prev_size_and_state_ |= STATE_BITMASK;
}

bool segment::is_unused() const
{
	return (0 == (prev_size_and_state_ & STATE_BITMASK));
}

void segment::set_unused()
{
	prev_size_and_state_ &= PREV_SIZE_BITMASK;
}

void segment::set_prev_size_and_used(size_type prev_size)
{
	prev_size_and_state_ = STATE_BITMASK | (prev_size_and_state_ & PREV_SIZE_BITMASK);
}

void segment::set_prev_size_and_unused(size_type prev_size)
{
	prev_size_and_state_ = prev_size & PREV_SIZE_BITMASK;
}

bool segment::is_first_segment(const uint8_t *storage_begin) const
{
	return ((uint8_t *)this == storage_begin);
}

bool segment::is_last_segment(const uint8_t *storage_end) const
{
	return ((uint8_t *)this + get_size() >= storage_end);
}

result<segment *> segment::get_prev(const uint8_t *storage_begin)
{
	if (is_first_segment(storage_begin))
	{
		return fail(error::out_of_bounds);
	}
	else
	{
		return success((segment *)((uint8_t *)this - get_prev_size()));
	}
}

result<const segment *> segment::get_prev(const uint8_t *storage_begin) const
{
	if (is_first_segment(storage_begin))
	{
		return fail(error::out_of_bounds);
	}
	else
	{
		return success((const segment *)((const uint8_t *)this - get_prev_size()));
	}
}

result<segment *> segment::get_next(const uint8_t *storage_end)
{
	if (is_last_segment(storage_end))
	{
		return fail(error::out_of_bounds);
	}
	else
	{
		return success((segment *)((uint8_t *)this + get_size()));
	}
}

result<const segment *> segment::get_next(const uint8_t *storage_end) const
{
	if (is_last_segment(storage_end))
	{
		return fail(error::out_of_bounds);
	}
	else
	{
		return success((const segment *)((const uint8_t *)this + get_size()));
	}
}

// split an unused segment B into B1, B2 (B will became B1 and will be set to used and B2 will be set to unused and 
// will be returned)
result<segment *> segment::split_unused_segment(size_type size, const uint8_t *storage_end)
{
	auto   crsize = get_size();

	if (size >= crsize)
	{
		return fail(error::invalid_arg);
	}
	if (size > MAX_SIZE)
	{
		return fail(error::invalid_arg);
	}

	auto   next_size = crsize - size;

	if (next_size < MIN_SIZE)
	{
		return fail(error::invalid_arg);
	}

	set_size(size);
	set_used();

	segment   *next_segment = (segment *)((uint8_t *)this + size);

	next_segment->set_size(next_size);
	next_segment->set_prev_size_and_unused(size);

	if (auto res = next_segment->get_next(storage_end); res)
	{
		res.value()->set_prev_size(next_size);
	}

	return success(next_segment);
}

result<> segment::merge(segment *seg, segment *next_segment, const uint8_t *storage_end)
{
	auto   size      = seg->get_size();
	auto   next_size = next_segment->get_size();

	if (MAX_SIZE - size < next_size)
	{
		return fail(error::out_of_bounds);
	}

	auto   newsize = size + next_size;

	seg->set_size(newsize);

	if (auto res = next_segment->get_next(storage_end); res)
	{
		res.value()->set_prev_size(newsize);
	}

	return success();
}

// expand the segment into prev and/or next segment if those segments are unused and set it as unused; 
// return resulting segment
result<segment *> segment::expand_segment(const uint8_t *storage_begin, const uint8_t *storage_end, 
                                          uint32_t *merge_info/* = nullptr*/)
{
	segment   *expanded_segment = nullptr;

	if (nullptr != merge_info)
	{
		*merge_info = 0;
	}

	if (auto res = get_next(storage_end); res && res.value()->is_unused())
	{
		if (auto r = merge(this, res.value(), storage_end); r)
		{
			expanded_segment = this;
			if (nullptr != merge_info)
			{
				*merge_info |= MERGE_NEXT;
			}
		}
		else
		{
			return fail(r.error());
		}
	}
	if (auto res = get_prev(storage_begin); res && res.value()->is_unused())
	{
		if (auto r = merge(res.value(), this, storage_end); r)
		{
			expanded_segment = res.value();
			if (nullptr != merge_info)
			{
				*merge_info |= MERGE_PREV;
			}
		}
		else
		{
			return fail(r.error());
		}
	}

	if (nullptr == expanded_segment)
	{
		return fail(error::not_available);
	}

	expanded_segment->set_unused();

	return success(expanded_segment);
}

}//namespace meanbot::mempack::storage
