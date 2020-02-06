#pragma once
#include "framework.h"
#include <string>

namespace helper
{
	namespace window
	{
		//get exact dementions of certain window.
		RECT GetRect(const HWND& hWnd);
	}

	namespace strings
	{
		//wstring + int + wstring
		template <typename T>
		std::wstring wsinttows(const std::wstring& orig, const T& i)
		{
			return orig + std::to_wstring(i);
		}
		//std::wstring wsinttows(const std::wstring& orig, const float& i);
		std::wstring GetDirectoryFromPath(const std::wstring& filepath);
		std::wstring GetNameFromPath(const std::wstring& filepath);
		std::wstring GetFileExtention(const std::wstring& filename);
		std::wstring charToWide(const std::string& str);
		std::string wideToChar(const std::wstring& wstr);
	}

};

