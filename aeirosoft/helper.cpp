#include "helper.h"
#include <algorithm>
#include <sstream>
//get exact dementions of certain window.
RECT helper::window::GetRect(const HWND& hWnd)
{
	RECT r = { 0 };
	GetWindowRect(hWnd, &r);
	r.right = r.right - r.left;
	r.left = 0;
	r.bottom = r.bottom - r.top;
	r.top = 0;
	return r;
}

//wstring + int + wstring



std::wstring helper::strings::GetDirectoryFromPath(const std::wstring& filepath)
{
	size_t offset1 = filepath.find_last_of('\\');
	size_t offset2 = filepath.find_last_of('/');

	if (offset1 == std::wstring::npos && offset2 == std::wstring::npos)
		return L"";
	if (offset1 == std::wstring::npos)
		return filepath.substr(0, offset2);
	if (offset2 == std::wstring::npos)
		return filepath.substr(0, offset1);

	return filepath.substr(0, std::max(offset1, offset2));

}

std::wstring helper::strings::GetNameFromPath(const std::wstring& filepath)
{
	size_t offset1 = filepath.find_last_of('\\');
	size_t offset2 = filepath.find_last_of('/');

	if (offset1 == std::wstring::npos && offset2 == std::wstring::npos)
		return L"";
	if (offset1 == std::wstring::npos)
		return filepath.substr(offset2, filepath.length());
	if (offset2 == std::wstring::npos)
		return filepath.substr(offset1 + 1, filepath.length());

	return filepath.substr(std::max(offset1 + 1, offset2), filepath.length());

}

std::wstring helper::strings::GetFileExtention(const std::wstring& filename)
{
	size_t offset = filename.find_last_of('.');
	if (offset == std::wstring::npos)
		return L"";
	return std::wstring(filename.substr(offset + 1));
}
std::wstring helper::strings::charToWide(const std::string& str)
{
	using namespace std;
	std::wstring wstr;
	size_t out;
	wstr.resize(str.length() + 1);
	mbstowcs_s(&out, wstr.data(), wstr.length(), str.data(), str.length());
	return wstr;
}
std::string helper::strings::wideToChar(const std::wstring& wstr)
{
	using namespace std;
	std::string str;
	size_t out;
	str.resize(wstr.length() + 1);
	wcstombs_s(&out , str.data(), str.length(), wstr.data(), wstr.length());
	return str;
}

