#include "TextEvent.hpp"

namespace wv
{


	TextEvent::TextEvent(const sf::Vector2f pos, const std::string & script)
		:IEvent(), m_script(script), m_hitBox(sf::RectangleShape(sf::Vector2f(16.0f, 16.0f)))
	{
		setPosition(pos);
		m_hitBox.setPosition(pos);
		m_hitBox.setFillColor(sf::Color::Transparent);
		m_hitBox.setOutlineColor(sf::Color::Transparent);
	}

	TextEvent::~TextEvent()
	{
	}

	IHitBox & TextEvent::getHitBox()
	{
		return m_hitBox;
	}

	void TextEvent::collided(Player & p)
	{
		TextBox* tb = p.getTextBox();
		if (tb)
		{
			tb->loadFile(m_script);
			tb->show();
		}
		done();
	}
	const sf::Drawable & TextEvent::getDrawable(const sf::View &)
	{
		if (m_hitBox.isVisible())
		{
			m_hitBox.setOutlineColor(sf::Color::Red);
		}
		else
		{
			m_hitBox.setOutlineColor(sf::Color::Transparent);
		}
		return m_hitBox;
	}
}