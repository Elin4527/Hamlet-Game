#include "OverworldState.hpp"
#include <iostream>
#include "../Items/Fist.hpp"
#include "../Enemies/EvilHamlet.hpp"
#include "../../WildVEngine/WildVEngine.hpp"
#include "VictoryState.hpp"
#include "DeadState.hpp"

void OverworldState::entered()
{
	// Set up the view
	m_view.reset(sf::FloatRect(0, 0, static_cast<float>(wv::IApp::instance()->getWidth()), static_cast<float>(wv::IApp::instance()->getHeight())));

	// Create the tile map
	m_tilemap.reset(new wv::TileMap);
	if (!m_tilemap->loadTileMap("Data/Level/room1.txt"))
	{
		std::cout << "Map loading failed" << std::endl;
	}

	m_renderables.push_back(m_tilemap.get());

	// Create the player and the GUI
	auto spawn = m_tilemap->getSpawnPoint();
	m_player = std::make_shared<wv::Player>(m_tilemap.get(), spawn.x, spawn.y, 5);
	m_player->loadFromFile("Data/Characters/hamlet.txt");
	m_tilemap->setPlayer(m_player);

	m_renderables.push_back(m_player.get());
	m_GUI.reset(new wv::PlayerGUI(*m_player));
	m_renderables.push_back(m_GUI.get());

	// Initialize the textbox for message display
	m_textBox.reset(new wv::TextBox(m_tilemap.get(), "Font/AdobeGothicStd-Bold.otf", "Images/GUI/textArea.png", 12, 10, false));
	m_textBox->setLocation(sf::Vector2f(0.0f, 380.f));
	m_textBox->setColor(sf::Color::White);
	m_player->setTextBox(m_textBox.get());
	m_renderables.push_back(m_textBox.get());
}

void OverworldState::exiting()
{

}

void OverworldState::obscuring()
{
	m_tilemap->queueMapChange(m_tilemap->getLoadedMap());
}

void OverworldState::revealed()
{
	m_player->heal(1000);
}

bool OverworldState::handleInput(sf::Event & e)
{	
	if (m_textBox->isVisible())
	{
		return m_textBox->handleInput(e);
	}
	else
	{
		return m_player->handleInput(e);
	}
}

void OverworldState::fixedUpdate()
{
	m_textBox->fixedUpdate();

	m_tilemap->fixedUpdate();

	if (m_player->isDead())
	{
		setAdd(std::make_shared<DeadState>(), wv::modality::EXCLUSIVE);
		
	}
	if (m_textBox->gameOver())
	{
		setChange(std::make_shared<VictoryState>(), wv::modality::EXCLUSIVE);
	}
}

void OverworldState::update()
{
	m_tilemap->update();

}

void OverworldState::lateUpdate()
{
	m_view.setCenter(m_player->getCenter());
}

void OverworldState::render(sf::RenderTarget & rt)
{
	rt.setView(m_view);
	for (auto& r : m_renderables)
	{
		const sf::Drawable& d = r->getDrawable(m_view);
		const sf::RenderStates& t = r->getTransform();
		rt.draw(d, t);
	}
}
