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
#include <string>


namespace meanbot::mempack
{

// https://semver.org
struct version final
{
	static constexpr std::string_view   NAME          = "meanbot.mempack";
	static constexpr std::string_view   DESCRIPTION   = "persistent memory allocator";
	static constexpr std::string_view   URL           = "https://github.com/meanbot/mempack";

	static constexpr uint32_t           MAJOR         = 0; // when you make incompatible API changes
	static constexpr uint32_t           MINOR         = 0; // when you add functionality in a backward compatible manner
	static constexpr uint32_t           PATCH         = 1; // when you make backward compatible bug fixes
	static constexpr std::string_view   PRERELEASE    = "";
	static constexpr std::string_view   BUILDMETADATA = "";

	// <MAJOR>.<MINOR>.<PATCH>[-<PRERELEASE>][+BUILDMETADATA]
	static constexpr std::string_view   STRING        = "0.0.1";
};//struct version

}//namespace meanbot::mempack
