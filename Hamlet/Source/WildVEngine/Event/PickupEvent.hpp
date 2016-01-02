#pragma once

#include "IEvent.hpp"
#include <memory>
#include "../Collision/THitbox.hpp"
#include "../Interfaces/TResource.hpp"
#include "../Graphics2D/AnimatedSprite.hpp"

namespace wv
{
	// Adds an item to the players inventory when stepped on
	class PickUpEvent : public IEvent
	{
		static std::shared_ptr<wv::Animation> g_anim;
		static unsigned int g_ref;

		THitBox<sf::CircleShape> m_hitBox;
		std::string m_itemName;
		std::string m_script;
		AnimatedSprite m_sprite;
	public:
		PickUpEvent(const sf::Vector2f pos, const std::string& item, const std::string& script);
		virtual ~PickUpEvent();

		virtual IHitBox& getHitBox() override;
		virtual void collided(Player& p) override;
		virtual const sf::Drawable & getDrawable(const sf::View &) override;
		virtual void update() override;
	};

}