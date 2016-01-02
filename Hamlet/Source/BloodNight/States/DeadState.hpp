#pragma once

#include "../../WildVEngine/WildVEngine.hpp"

class DeadState : public wv::IState,
	public virtual wv::Updateable, public virtual wv::Renderable
{
	sf::View* m_view;
	sf::Sprite* m_bg;
	wv::TResource<sf::Texture> m_background;
	wv::TResource<sf::Font> m_font;
	std::unique_ptr<wv::TextBox> m_textBox;
	std::vector<sf::Text*> m_options;
	unsigned int m_choice;
public:
	virtual void entered() override;
	virtual void exiting() override;

	virtual bool handleInput(sf::Event& e) override;
	virtual void fixedUpdate() override;
	virtual void render(sf::RenderTarget & rt) override;

};