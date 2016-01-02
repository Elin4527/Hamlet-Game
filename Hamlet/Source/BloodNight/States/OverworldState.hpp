#pragma once
#include "../../WildVEngine/Interfaces/IState.hpp"
#include "../../WildVEngine/Object/TileMap.hpp"
#include "../../WildVEngine/Interfaces/IApp.hpp"
#include "../../WildVEngine/Object/Player.hpp"
#include "../../WildVEngine/Object/TextBox.hpp"
#include "../../WildVEngine/Object/PlayerGUI.hpp"

class OverworldState : public wv::IState,
	public virtual wv::Renderable, public virtual wv::Updateable
{
	sf::View m_view;
	std::unique_ptr<wv::TileMap> m_tilemap;
	std::shared_ptr<wv::Player> m_player;
	std::unique_ptr<wv::TextBox> m_textBox;
	std::unique_ptr<wv::PlayerGUI> m_GUI;
	std::vector<wv::RenderComponent*> m_renderables;

public:
	virtual void entered() override;
	virtual void exiting() override;
	virtual void obscuring() override;
	virtual void revealed() override;
	virtual bool handleInput(sf::Event& e) override;
	virtual void fixedUpdate() override;
	virtual void lateUpdate() override;
	virtual void update() override;
	virtual void render(sf::RenderTarget& rt) override;
};