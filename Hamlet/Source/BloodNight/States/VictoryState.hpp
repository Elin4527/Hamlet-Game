#pragma once

#include "../../WildVEngine/WildVEngine.hpp"


class VictoryState : public wv::IState, 
	public virtual wv::Renderable, virtual public wv::Updateable
{
	sf::View* m_view;
	sf::Sprite* m_bg;
	wv::TResource<sf::Texture> m_background;
	wv::TResource<sf::Font> m_font;
	std::unique_ptr<wv::TextBox> m_textBox;
	sf::Text* m_end;
	unsigned int m_choice;
public:
	virtual void entered() override;
	virtual void exiting() override;

	virtual bool handleInput(sf::Event& e) override;
	virtual void fixedUpdate() override;
	virtual void render(sf::RenderTarget & rt) override;
};
