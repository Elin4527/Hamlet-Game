#pragma once

#include "IEvent.hpp"
#include "../Collision/THitbox.hpp"

namespace wv
{
	// Opens up the TextBox and passes it a script when activated
	class TextEvent : public IEvent
	{
		THitBox<sf::RectangleShape> m_hitBox;
		std::string m_script;
	public:
		TextEvent(const sf::Vector2f pos, const std::string& script);
		virtual ~TextEvent();

		virtual void collided(Player& p) override;
		virtual const sf::Drawable & getDrawable(const sf::View &) override;
		virtual IHitBox & getHitBox() override;
	};

}