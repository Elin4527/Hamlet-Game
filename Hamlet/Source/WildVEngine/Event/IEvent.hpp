#pragma once

#include "../Object/Player.hpp"
#include "../Object/TextBox.hpp"
#include "../Collision/IHitbox.hpp"
#include <memory>

namespace wv
{
	// Events that are processed throught the TileMap (really rushed so no proper Event handling class)
	class IEvent : public RenderComponent
	{
		bool m_active;
	public:
		virtual ~IEvent();

		virtual IHitBox& getHitBox() = 0;

		virtual void collided(Player& c);
		virtual void interact(Player& c);

		virtual void setColor(const sf::Color & c) override;
		bool isActive() const;

	protected:
		IEvent();
		void done();
	};

}
