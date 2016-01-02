#pragma once

#include "..\..\WildVEngine\Interfaces\IState.hpp"
#include "..\..\WildVEngine\Interfaces\TResource.hpp"

class MainMenuState : public wv::IState, 
	public virtual wv::Updateable, public virtual wv::Renderable
{
	sf::View* m_view;
	sf::Sprite* m_bg;
	wv::TResource<sf::Texture> m_background;
	wv::TResource<sf::Font> m_font;
	sf::Text* m_title;
	std::vector<sf::Text*> m_options;
	unsigned int m_choice;
public:
	virtual void entered() override;
	virtual void exiting() override;

	virtual bool handleInput(sf::Event& e) override;
	virtual void render(sf::RenderTarget & rt) override;

};