
#include "color.h"

color::color()
	:Color(0)
{}

color::color(unsigned int val) :
	Color(val)
{}

color::color(UCHAR r, UCHAR g, UCHAR b) :
	color(r, g, b, 255)
{}

color::color(UCHAR r, UCHAR g, UCHAR b, UCHAR a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

color::color(const color& src) :
	color(src.Color)
{}

color& color::operator=(const color& src)
{
	Color = src.Color;
	return *this;
}

bool color::operator==(const color& rhs) const
{
	return (this->Color == rhs.Color);
}

bool color::operator!=(const color& rhs) const
{
	return (this->Color != rhs.Color);
}

constexpr UCHAR color::getR() const
{
	return rgba[0];
}

void color::setR(UCHAR r)
{
	rgba[0] = r;
}

constexpr UCHAR color::getG() const
{
	return rgba[1];
}

void color::setG(UCHAR g)
{
	rgba[1] = g;
}

constexpr UCHAR color::getB() const
{
	return rgba[2];
}

void color::setB(UCHAR b)
{
	rgba[2] = b;
}

constexpr UCHAR color::getA() const
{
	return rgba[3];
}

void color::setA(UCHAR a)
{
	rgba[3] = a;
}
