#pragma once
#include "IObject.hpp"
#include "Player.hpp"
#include "../Interfaces/TResource.hpp"

namespace wv
{

	class PlayerGUI : public RenderComponent
	{
		sf::RenderTexture m_render;
		sf::Sprite m_drawable;
		TResource<sf::Texture> m_health;
		TResource<sf::Texture> m_skill;
		TResource<sf::Texture> m_item;
		TResource<sf::Font> m_font;
		Player& m_player;
	public:
		PlayerGUI(Player& p);

		virtual void setColor(const sf::Color & c) override;

		virtual const sf::Drawable & getDrawable(const sf::View &) override;

	};

}