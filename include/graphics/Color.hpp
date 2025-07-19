#ifndef GRAPHICS_COLOR_HPP
#define GRAPHICS_COLOR_HPP

struct Color
{
private:
	float m_red;
	float m_green;
	float m_blue;
	float m_alpha;

public:
	enum Channel
	{
		R,
		G,
		B,
		A
	};

	Color(const float &r, const float &g, const float &b, const float &a);
	Color(const float &r, const float &g, const float &b);
	float get(const Channel &c);
};

#endif // GRAPHICS_COLOR_HPP