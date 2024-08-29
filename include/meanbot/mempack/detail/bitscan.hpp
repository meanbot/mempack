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

// on msvc these are just as fast as the provided intrinsics for one call, but much faster for multiple calls in a row

/*
AMD Ryzen 9 3900X
Run on (24 X 3793 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x12)
  L1 Instruction 32 KiB (x12)
  L2 Unified 512 KiB (x12)
  L3 Unified 16384 KiB (x4)

32 bit:
---------------------------------------------------------------------------------
Benchmark                                       Time             CPU   Iterations
---------------------------------------------------------------------------------
benchmark_bitscanforward_32_table::run       1.17 ns         1.17 ns    640000000
benchmark_bitscanforward_32_msvc::run        1.41 ns         1.40 ns    448000000
benchmark_bitscanforward_64_table::run       1.15 ns         1.14 ns    560000000
benchmark_bitscanreverse_32_table::run       1.15 ns         1.13 ns    497777778
benchmark_bitscanreverse_32_msvc::run        1.75 ns         1.67 ns    448000000
benchmark_bitscanreverse_64_table::run       1.16 ns         1.17 ns    560000000

64 bit
---------------------------------------------------------------------------------
Benchmark                                       Time             CPU   Iterations
---------------------------------------------------------------------------------
benchmark_bitscanforward_32_table::run       1.18 ns         1.14 ns    560000000
benchmark_bitscanforward_32_msvc::run        1.17 ns         1.17 ns    560000000
benchmark_bitscanforward_64_table::run       1.40 ns         1.41 ns    497777778
benchmark_bitscanforward_64_msvc::run        1.38 ns         1.38 ns    497777778
benchmark_bitscanreverse_32_table::run       1.18 ns         1.14 ns    560000000
benchmark_bitscanreverse_32_msvc::run        1.38 ns         1.38 ns    497777778
benchmark_bitscanreverse_64_table::run       1.36 ns         1.35 ns    497777778
benchmark_bitscanreverse_64_msvc::run        1.47 ns         1.29 ns    497777778
*/
struct bitscan final
{
	//https://www.chessprogramming.org/Matt_Taylor

	static inline const int LSB_64_TABLE[64] =
	{
		63, 30,  3, 32, 59, 14, 11, 33,
		60, 24, 50,  9, 55, 19, 21, 34,
		61, 29,  2, 53, 51, 23, 41, 18,
		56, 28,  1, 43, 46, 27,  0, 35,
		62, 31, 58,  4,  5, 49, 54,  6,
		15, 52, 12, 40,  7, 42, 45, 16,
		25, 57, 48, 13, 10, 39,  8, 44,
		20, 47, 38, 22, 17, 37, 36, 26
	};

	/**
	 * bitScanForward
	 * @author Matt Taylor (2003)
	 * @param bb bitboard to scan
	 * @precondition bb != 0
	 * @return index (0..63) of least significant one bit
	 */
	static inline uint32_t forward64(uint64_t value)
	{
		uint32_t   folded = 0;

		value ^= value - 1;
		folded = (int) value ^ (value >> 32);

		return LSB_64_TABLE[folded * 0x78291ACF >> 26];
	}

	static inline uint32_t forward32(uint32_t value)
	{
		return forward64(value);
	}

	// https://www.chessprogramming.org/BitScan#De_Bruijn_Multiplication_2

	static inline const uint32_t INDEX64[64] = 
	{
		 0, 47,  1, 56, 48, 27,  2, 60,
		57, 49, 41, 37, 28, 16,  3, 61,
		54, 58, 35, 52, 50, 42, 21, 44,
		38, 32, 29, 23, 17, 11,  4, 62,
		46, 55, 26, 59, 40, 36, 15, 53,
		34, 51, 20, 43, 31, 22, 10, 45,
		25, 39, 14, 33, 19, 30,  9, 24,
		13, 18,  8, 12,  7,  6,  5, 63
	};

	/**
	 * bitScanReverse
	 * @authors Kim Walisch, Mark Dickinson
	 * @param bb bitboard to scan
	 * @precondition bb != 0
	 * @return index (0..63) of most significant one bit
	 */
	static inline uint32_t reverse64(uint64_t value)
	{
		const unsigned __int64 debruijn64 = 0x03f79d71b4cb0a89;

		value |= value >> 1; 
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		value |= value >> 32;

		return INDEX64[(value * debruijn64) >> 58];
	}

	static inline uint32_t reverse32(uint32_t value)
	{
		return reverse64(value);
	}
};//struct bitscan final

}//namespace meanbot::mempack::detail
