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
#include "meanbot/mempack/storage/segments_sorted_array.hpp"


namespace meanbot::mempack::storage
{

result<size_type> segments_sorted_array::get_mem_size(size_type total_segment_count)
{
	if (0 == total_segment_count)
	{
		return fail(error::invalid_arg);
	}
	if (sizeof(entry) > (segment::MAX_SIZE - sizeof(segments_sorted_array)) / (total_segment_count - 1))
	{
		return fail(error::too_big);
	}

	return detail::byte_alignment::align(sizeof(segments_sorted_array) + (total_segment_count - 1) * sizeof(entry), 
	                                     detail::config::ALIGNMENT);
}

result<> segments_sorted_array::init(size_type total_segments_count)
{
	total_count_ = total_segments_count;
	used_count_  = 0;
	memset(entries_, 0, sizeof(entry) * total_count_);

	return success();
}

bool segments_sorted_array::search(uint64_t &index, uint64_t size, uint64_t offset/* = INVALID_SIZE*/)
{
	if (0 == used_count_)
	{
		index = 0;

		return false;
	}
	else
	{
		uint64_t   begin  = 0;
		uint64_t   end    = used_count_ - 1;
		uint64_t   middle = 0;

		for (;;)
		{
			middle = begin + ((end - begin + 1) / 2);
			if (size <entries_[middle].size_)
			{
				if (begin == middle)
				{
					index = middle;
	
					return false;
				}
				else
				{
					end = middle - 1;
				}
			}
			else
			if (size > entries_[middle].size_)
			{
				if (end == middle)
				{
					index = middle + 1;

					return false;
				}
				else
				{
					begin = middle + 1;
				}
			}
			else
			{
				if (INVALID_SIZE == offset)
				{
					index = middle;

					return true;
				}
				else
				{
					if (offset < entries_[middle].offset_)
					{
						if (begin == middle)
						{
							index = middle;

							return false;
						}
						else
						{
							end = middle - 1;
						}
					}
					else
					if (offset > entries_[middle].offset_)
					{
						if (end == middle)
						{
							index = middle + 1;
							
							return  false;
						}
						else
						{
							begin = middle + 1;
						}
					}
					else
					{
						index = middle;

						return true;
					}
				}
			}
		}
	}
}

result<> segments_sorted_array::insert_at_index(uint64_t index, uint64_t size, uint64_t offset)
{
	if (used_count_ == total_count_)
	{
		return fail(error::out_of_bounds);
	}
	if (index < used_count_)
	{
		memmove(entries_ + index + 1, entries_ + index, sizeof(entry) * (used_count_ - index));
	}
	entries_[index].size_   = size;
	entries_[index].offset_ = offset;
	used_count_++;

	return success();
}

result<> segments_sorted_array::remove_at_index(uint64_t index)
{
	if (index < used_count_ - 1)
	{
		memmove(entries_ + index, entries_ + index + 1, sizeof(entry) * (used_count_ - index - 1));
	}
	used_count_--;

	return success();
}

result<> segments_sorted_array::add(size_type size, size_type offset)
{
	uint64_t   index = 0;

	if (search(index, size, offset))
	{
		return fail(error::duplicate);
	}
	if (auto res = insert_at_index(index, size, offset); !res)
	{
		return fail(res.error());
	}

	return success();
}

// remove an unused exact segment
result<> segments_sorted_array::remove(size_type size, size_type offset)
{
	uint64_t   index = 0;

	if (!search(index, size, offset))
	{
		return fail(error::not_found);
	}
	if (auto res = remove_at_index(index); !res)
	{
		return fail(res.error());
	}

	return success();
}

// remove an unused segment that has at least <size> size
result<> segments_sorted_array::remove(size_type size, uint64_t &found_size, uint64_t &found_offset)
{
	uint64_t   index = 0;

	if (!search(index, size))
	{
		if (index < used_count_)
		{
			if (entries_[index].size_ < size)
			{
				if (index + 1 < used_count_ && entries_[index + 1].size_ < size)
				{
					index++;
				}
				else
				{
					return fail(error::not_found);
				}
			}
		}
	}
	found_size   = entries_[index].size_;
	found_offset = entries_[index].offset_;
	if (auto res = remove_at_index(index); !res)
	{
		return fail(res.error());
	}

	return success();
}

}//namespace meanbot::mempack::storage
