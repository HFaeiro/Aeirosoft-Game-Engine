#pragma once
typedef unsigned char UCHAR;

class color
{
public:
	color();
	color(unsigned int val);
	color(UCHAR r, UCHAR g, UCHAR b);
	color(UCHAR r, UCHAR g, UCHAR b, UCHAR a);
	color(const color& src);

	color& operator = (const color& src);
	bool operator == (const color& rhs) const;
	bool operator != (const color& rhs) const;

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
		unsigned int Color;
	};

};

namespace colors
{
	const color UnloadedTexturecolor(100, 100, 100);
	const color UnhandledTexturecolor(250, 0, 0);
}