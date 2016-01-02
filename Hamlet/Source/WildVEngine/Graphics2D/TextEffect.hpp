#pragma once
#include "DynamicGraphic.hpp"
#include "../Interfaces/TResource.hpp"

namespace wv
{
	// Text with effects updated every frame
	class TextEffect : public DynamicGraphic
	{
		TResource<sf::Font> m_font;
		sf::Text m_text;
		sf::Time m_lifespan;
		sf::Time m_timeRemaining;
		sf::Vector2f m_translation;
		sf::Color m_startColor;
		sf::Color m_endColor;
	public:
		TextEffect(const std::string& str, const std::string& font, unsigned int ptSize, const sf::Vector2f& pos, sf::Time lifespan, 
			float change, float radians, const sf::Color& start = sf::Color::White, const sf::Color& end = sf::Color::Transparent);

		virtual void update(sf::Time fd) override;
		virtual bool isPlaying() const override;
	private:

		// Inherited via DynamicGraphic
		virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	};
}