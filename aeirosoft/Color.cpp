
#include "Color.h"

Color::Color()
	:color(0)
{}

Color::Color(unsigned int val):
	color(val)
{}

Color::Color(UCHAR r, UCHAR g, UCHAR b):
	Color(r,g,b,255)
{}

Color::Color(UCHAR r, UCHAR g, UCHAR b, UCHAR a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

Color::Color(const Color& src):
	color(src.color)
{}

Color& Color::operator=(const Color & src)
{
	color = src.color;
	return *this;
}

bool Color::operator==(const Color& rhs) const
{
	return (this->color == rhs.color);
}

bool Color::operator!=(const Color& rhs) const
{
	return (this->color != rhs.color);
}

constexpr UCHAR Color::getR() const
{
	return rgba[0];
}

void Color::setR(UCHAR r)
{
	rgba[0] = r;
}

constexpr UCHAR Color::getG() const
{
	return rgba[1];
}

void Color::setG(UCHAR g)
{
	rgba[1] = g;
}

constexpr UCHAR Color::getB() const
{
	return rgba[2];
}

void Color::setB(UCHAR b)
{
	rgba[2] = b;
}

constexpr UCHAR Color::getA() const
{
	return rgba[3];
}

void Color::setA(UCHAR a)
{
	rgba[3] = a;
}
