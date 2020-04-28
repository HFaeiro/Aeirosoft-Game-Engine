#pragma once
#include "framework.h"
#include <string>
#include "Vertex.h"

namespace helper
{
	static bool _compare_min_x(Vertex const& p1, Vertex const& p2) { return p1.position.x < p2.position.x; }
	static  bool _compare_min_y(Vertex const& p1, Vertex const& p2) { return p1.position.y < p2.position.y; }
	static bool _compare_min_z(Vertex const& p1, Vertex const& p2) { return p1.position.z < p2.position.z; }

	static bool _compare_max_x(Vertex const& p1, Vertex const& p2) { return p1.position.x > p2.position.x; }
	static  bool _compare_max_y(Vertex const& p1, Vertex const& p2) { return p1.position.y > p2.position.y; }
	static bool _compare_max_z(Vertex const& p1, Vertex const& p2) { return p1.position.z > p2.position.z; }

	static bool _compare_min_float(float const& x, float const& x1) { return x < x1; }



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

