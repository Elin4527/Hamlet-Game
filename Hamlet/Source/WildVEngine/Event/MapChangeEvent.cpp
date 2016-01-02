#include "MapChangeEvent.hpp"

namespace wv
{
	std::shared_ptr<Animation> MapChangeEvent::g_anim = nullptr;
	unsigned int MapChangeEvent::g_ref = 0;

	MapChangeEvent::MapChangeEvent(const sf::Vector2f pos, const std::string& map)
		:IEvent(), m_mapName(map), m_hitBox(sf::RectangleShape(sf::Vector2f(16.f, 16.f)))
	{
		setPosition(pos);
		m_hitBox.setPosition(pos);

		g_ref++;
		if (!g_anim)
		{
			g_anim = std::make_shared<wv::Animation>("Images/Animations/teleport.png");
			g_anim->addFrame(sf::IntRect(0, 0, 16, 16));
			g_anim->addFrame(sf::IntRect(16, 0, 16, 16));
			g_anim->addFrame(sf::IntRect(32, 0, 16, 16));
			g_anim->addFrame(sf::IntRect(48, 0, 16, 16));
		}

		m_sprite.setAnimation(g_anim);
		m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);
	}

	MapChangeEvent::~MapChangeEvent()
	{
		g_ref--;
		if (!g_ref)
		{
			g_anim = nullptr;
		}
	}

	IHitBox & MapChangeEvent::getHitBox()
	{
		return m_hitBox;
	}

	void MapChangeEvent::update()
	{
		m_sprite.update(IApp::instance()->getTime().getDeltaRenderTime());
	}

	void MapChangeEvent::collided(Player & p)
	{
		p.getTileMap()->queueMapChange(m_mapName);
		done();
	}
	const sf::Drawable & MapChangeEvent::getDrawable(const sf::View &)
	{
		return m_sprite;
	}
}