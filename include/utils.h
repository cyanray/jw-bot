#pragma once
#ifndef CYANRAY_UTILS_LIBRARY_UTILS_H_
#define CYANRAY_UTILS_LIBRARY_UTILS_H_
#include <string>
#include <locale>
#include <string_view>
#include <concepts>

namespace Cyan::Encoding::UTF8
{
	namespace
	{
		template<typename T>
		std::wstring to_wstring_impl(std::basic_string_view<T> str)
		{
			std::wstring wstr;
			wstr.reserve(str.size());
			auto r = std::mbstowcs(const_cast<wchar_t*>(wstr.data()), reinterpret_cast<const char*>(str.data()), str.size());
			return std::wstring(wstr.data());
		}
	}

	std::wstring to_wstring(std::u8string_view str)
	{
		return to_wstring_impl(str);
	}

	std::wstring to_wstring(std::string_view str)
	{
		return to_wstring_impl(str);
	}
}

namespace Cyan::Utils
{
	namespace
	{
		struct ChineseNumberHelper
		{
			std::array<int, 50> value{};
			constexpr ChineseNumberHelper()
			{
				std::wstring_view w = L"零一二三四五六七八九十百千万亿";
				std::array v = { 0,1,2,3,4,5,6,7,8,9,10,100,1000,10000,100000000 };
				value.fill(-1);
				for (size_t i = 0; i < w.size(); ++i)
				{
					value[Hash(w[i])] = v[i];
				}
			}

			constexpr static int Hash(wchar_t ch)
			{
				return (13 * 1123 + (unsigned int)ch) % 50;
			}
		};

		constexpr static ChineseNumberHelper ChineseNumberValue;

		constexpr int GetChineseNumberValue(wchar_t ch)
		{
			return ChineseNumberValue.value[ChineseNumberHelper::Hash(ch)];
		}
	}

	template<typename TResult>
	requires std::integral<TResult> || std::floating_point<TResult>
		constexpr TResult ChineseNumberTo(std::wstring_view w)
	{
		if (w.size() == 0) return 0;
		if (w.size() == 1) return GetChineseNumberValue(w[0]);
		TResult result = 0, last_num = 1, num = 0, section = 0;
		for (size_t i = 0; i < w.size(); ++i)
		{
			num = GetChineseNumberValue(w[i]);
			if (num == -1) [[unlikely]] return -1;
			if (num < 10)
			{
				last_num = num;
			}
			else if (num <= 1000)
			{
				section += last_num * num;
				last_num = 0;
			}
			else [[unlikely]]
			{
				result += (section + last_num) * num;
				section = 0;
			}
		}
		result += section + last_num;
		return result;
	}
}

#endif // !CYANRAY_UTILS_LIBRARY_UTILS_H_
