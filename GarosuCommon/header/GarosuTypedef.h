#pragma once
#ifndef __GAROSU_TYPEDEF_H__
#define __GAROSU_TYPEDEF_H__

#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace Garosu
{

	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;
	
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	using String = std::string;
	using WString = std::wstring;

	template <typename T>
	using uptr = std::unique_ptr<T>;

	template <typename T, typename... Args>
	inline uptr<T> mk_uptr(Args&&... args)
	{
		return std::make_unique<T>(args...);
	}

}

#endif