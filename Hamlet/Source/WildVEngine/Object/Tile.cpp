#include "Tile.hpp"

namespace wv
{
	Tile::Tile(std::shared_ptr<Animation> a, bool p)
		:m_passable(p)
	{
		m_sprite.setAnimation(a);
		m_sprite.setFrameTime(sf::seconds(1.0f / 4.0f));
	}

	Tile::~Tile()
	{
	}

	bool Tile::isPassable() const
	{
		return m_passable;
	}

	Tile* Tile::clone()
	{
		return new Tile(m_sprite.getAnimation(), m_passable);
	}

	void Tile::update()
	{
		m_sprite.update(IApp::instance()->getTime().getDeltaRenderTime());
	}

	const sf::Drawable & Tile::getDrawable(const sf::View&)
	{
		return m_sprite;
	}

	void Tile::setColor(const sf::Color & c)
	{
		m_sprite.setColor(c);
	}

}