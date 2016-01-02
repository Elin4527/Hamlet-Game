#pragma once
#include <SFML\Graphics.hpp>

namespace wv
{
	// Vector operations dot product, check for overlap of two lines, and normalizing vectors
	float dot(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
	bool overlap(const sf::Vector2f& lhs, const sf::Vector2f& rhs);
	sf::Vector2f normalize(const sf::Vector2f& v);
}