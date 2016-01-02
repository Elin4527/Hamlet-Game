#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <memory>
#include "../Interfaces/TResource.hpp"

namespace wv
{
	// Stores an animation as a list of frames with a spritesheet
	// Based on this: https://github.com/SFML/SFML/wiki/Source%3A-AnimatedSprite

	class Animation
	{
	public:
		Animation();
		Animation(const std::string& image);
		void addFrame(const sf::IntRect& f);
		void setSpriteSheet(const std::string& name);

		const sf::IntRect& getFrame(std::size_t i) const;
		const sf::Texture& getSpriteSheet();
		std::size_t size() const;
	private:
		std::vector<sf::IntRect> m_frames;
		TResource<sf::Texture> m_spriteSheet;
	};

}