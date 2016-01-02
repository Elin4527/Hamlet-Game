#pragma once
#include "IObject.hpp"
#include "../Graphics2D/AnimatedSprite.hpp"
#include "../Interfaces/IApp.hpp"


namespace wv
{

	class Tile : public RenderComponent
	{
		AnimatedSprite m_sprite;
		bool m_passable;
	public:
		Tile(std::shared_ptr<Animation> a, bool p);
		Tile(const Tile&) = delete;
		virtual ~Tile();

		// Override RenderComponent Methods
		virtual void update() override;
		virtual const sf::Drawable& getDrawable(const sf::View&) override;
		virtual void setColor(const sf::Color& c) override;

		bool isPassable() const;
		Tile* clone();
		
	};

}