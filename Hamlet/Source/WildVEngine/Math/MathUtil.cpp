#include "MathUtil.hpp"

namespace wv
{
	float dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
	{
		return lhs.x * rhs.x + lhs.y + rhs.y;
	}
	bool overlap(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
	{
		return (lhs.x <= rhs.y) && (rhs.x <= lhs.y);
	}
	sf::Vector2f normalize(const sf::Vector2f& v)
	{
		float mag = std::sqrt(v.x * v.x + v.y * v.y);
		return sf::Vector2f(v.x / mag, v.y / mag);
	}
}