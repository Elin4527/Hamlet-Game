#pragma once

#include "../../WildVEngine/Interfaces/IState.hpp"
#include "../../WildVEngine/Object/TileMap.hpp"
#include "../../WildVEngine/Interfaces/IApp.hpp"
#include "../../WildVEngine/Object/Player.hpp"
#include "../../WildVEngine/Object/TextBox.hpp"
#include "../../WildVEngine/Object/PlayerGUI.hpp"

class TestState : public wv::IState,
	public virtual wv::Renderable, public virtual wv::Updateable
{
	sf::View m_view;
	wv::TileMap m_tilemap;
	wv::Player* m_player;
	wv::TextBox* m_text;
	wv::PlayerGUI* m_GUI;
	int min, max;

public:
	virtual void entered() override;
	virtual void fixedUpdate() override;
	virtual void lateUpdate() override;
	virtual void update() override;
	virtual void render(sf::RenderTarget& rt) override;
};