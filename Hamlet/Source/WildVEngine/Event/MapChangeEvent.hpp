#pragma once
#include "IEvent.hpp"
#include "../Collision/THitbox.hpp"

namespace wv
{
	// Event that loads the next level
	// Appears as a special tile on the ground and changes the level when stepped on
	class MapChangeEvent : public IEvent
	{
		static std::shared_ptr<wv::Animation> g_anim;
		static unsigned int g_ref;

		THitBox<sf::RectangleShape> m_hitBox;
		std::string m_mapName;
		AnimatedSprite m_sprite;
	public:
		MapChangeEvent(const sf::Vector2f pos, const std::string& item);
		virtual ~MapChangeEvent();

		virtual void collided(Player& p) override;
		virtual const sf::Drawable & getDrawable(const sf::View &) override;
		virtual IHitBox & getHitBox() override;
		virtual void update() override;
	};

}