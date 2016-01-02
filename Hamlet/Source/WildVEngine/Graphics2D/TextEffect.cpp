#include "TextEffect.hpp"

namespace wv
{



	TextEffect::TextEffect(const std::string & str, const std::string & font, unsigned int ptSize, const sf::Vector2f& pos, sf::Time lifespan, float change, float radians, const sf::Color & start, const sf::Color & end)
		:m_font(font), m_text(), m_lifespan(lifespan), m_timeRemaining(lifespan), m_translation(change * std::cos(radians), change* std::sin(radians)), m_startColor(start), m_endColor(end)
	{
		m_text.setFont(m_font.getResource());
		m_text.setString(str);
		m_text.setCharacterSize(ptSize);
		m_text.setColor(start);
		m_text.setPosition(pos);
		m_text.setStyle(sf::Text::Bold);
		m_text.setOrigin(std::roundf(m_text.getLocalBounds().width / 2.0f), 0);
	}

	void TextEffect::update(sf::Time fd)
	{
		m_timeRemaining -= fd;
		if (m_timeRemaining < sf::Time::Zero)
		{
			m_timeRemaining = sf::Time::Zero;
		}

		m_text.setPosition(m_text.getPosition() + m_translation * fd.asSeconds());

		unsigned int interpol = static_cast<unsigned int>(std::roundf(((m_timeRemaining / m_lifespan) * 255)));
		unsigned int other = 255 - interpol;

		m_text.setColor(m_startColor * sf::Color(interpol, interpol, interpol, interpol) + m_endColor * sf::Color(other, other, other, other));
	}

	bool TextEffect::isPlaying() const
	{
		return m_timeRemaining != sf::Time::Zero;
	}

	void TextEffect::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		target.draw(m_text, states);
	}

}