#include "graphics/Color.hpp"

Color::Color(const float &r, const float &g, const float &b, const float &a) : m_red(r), m_green(g), m_blue(b), m_alpha(a) {}
Color::Color(const float &r, const float &g, const float &b) : Color(r, g, b, 1.f) {}

float Color::get(const Channel &c)
{
	switch (c)
	{
	case R:
		return m_red;
	case G:
		return m_green;
	case B:
		return m_blue;
	case A:
		return m_alpha;
	default:
		return -1.f;
	}
}