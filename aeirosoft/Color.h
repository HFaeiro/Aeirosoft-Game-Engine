#pragma once
typedef unsigned char UCHAR;

class Color
{
public:
	Color();
	Color(unsigned int val);
	Color(UCHAR r, UCHAR g, UCHAR b);
	Color(UCHAR r, UCHAR g, UCHAR b, UCHAR a);
	Color(const Color& src);

	Color& operator = (const Color& src);
	bool operator == (const Color& rhs) const;
	bool operator != (const Color& rhs) const;

	constexpr UCHAR getR() const;
	void setR(UCHAR r);

	constexpr UCHAR getG() const;
	void setG(UCHAR g);

	constexpr UCHAR getB() const;
	void setB(UCHAR b);

	constexpr UCHAR getA() const;
	void setA(UCHAR a);


private:
	union
	{
		UCHAR rgba[4];
		unsigned int color;
	};

};

namespace Colors
{
	const Color UnloadedTextureColor(100, 100, 100);
	const Color UnhandledTextureColor(250, 0, 0);
}