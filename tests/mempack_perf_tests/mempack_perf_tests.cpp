//   _ __ ___   ___ _ __ ___  _ __   __ _  ___| | __  
//  | '_ ` _ \ / _ \ '_ ` _ \| '_ \ / _` |/ __| |/ /  mempack - persistent memory allocator
//  | | | | | |  __/ | | | | | |_) | (_| | (__|   <   -------------------------------------
//  |_| |_| |_|\___|_| |_| |_| .__/ \__,_|\___|_|\_\  https://github.com/meanbot/mempack
//                           |_|                    
// 
// SPDX-FileCopyrightText: 2024 meanbot <https://github.com/meanbot>
// SPDX-License-Identifier: MIT

#include <benchmark/benchmark.h>
#include <fmt/core.h>


void benchmark_test(benchmark::State &state)
{
	for (auto _ : state)
	{
		int x = 0;

		x++;
		benchmark::DoNotOptimize(x);
	}
}

BENCHMARK(benchmark_test);

int main(int argc, char *argv[])
{
	int   result = 0;

	benchmark::Initialize(&argc, argv);
	if (benchmark::ReportUnrecognizedArguments(argc, argv))
	{
		result = 1;
	}
	else
	{
		benchmark::RunSpecifiedBenchmarks();
		benchmark::Shutdown();
	}

	return result;
}
