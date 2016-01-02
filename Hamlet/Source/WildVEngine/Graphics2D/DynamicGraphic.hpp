#pragma once
#include <SFML\Graphics.hpp>

namespace wv
{
	// Interface that represents Graphics that update visually every render frame
	class DynamicGraphic: public sf::Drawable, public sf::Transformable
	{
	public:
		virtual ~DynamicGraphic();
		virtual void update(sf::Time fd) = 0;
		virtual bool isPlaying() const = 0;
	};
}