#include "MainMenuState.hpp"
#include "../../WildVEngine/Interfaces/IApp.hpp"
#include "OverworldState.hpp"

void MainMenuState::entered()
{
	m_view = new sf::View(sf::FloatRect(0, 0, wv::IApp::instance()->getWidth(), wv::IApp::instance()->getHeight()));
	m_background.setID("Images/Backgrounds/menu.png");
	m_font.setID("Font/ARBERKLEY.ttf");

	m_bg = new sf::Sprite(m_background.getResource());
	m_title = new sf::Text("Hamlet", m_font.getResource(), 60);
	m_title->setOrigin(m_title->getLocalBounds().width / 2, 0);
	m_title->setPosition(wv::IApp::instance()->getWidth() / 2, 200);
	m_title->setColor(sf::Color(255, 255, 100));

	m_options.push_back(new sf::Text("Start", m_font.getResource(), 12));
	m_options[0]->setOrigin(m_options[0]->getLocalBounds().width / 2, 0);
	m_options[0]->setPosition(wv::IApp::instance()->getWidth() / 2, 300);
	m_options.push_back(new sf::Text("Exit", m_font.getResource(), 12));
	m_options[1]->setOrigin(m_options[1]->getLocalBounds().width / 2, 0);
	m_options[1]->setPosition(wv::IApp::instance()->getWidth() / 2, 320);
}

void MainMenuState::exiting()
{
	delete m_bg;
	delete m_title;
	for (auto o : m_options)
	{
		delete o;
	}
}

bool MainMenuState::handleInput(sf::Event & e)
{
	if (e.type == sf::Event::KeyPressed)
	{
		switch (e.key.code)
		{
		case sf::Keyboard::Up:
			if (m_choice == 0)
			{
				m_choice = m_options.size();
			}
			m_choice--;
			break;
		case sf::Keyboard::Down:
			m_choice++;
			if (m_choice == m_options.size())
			{
				m_choice = 0;
			}
			break;
		case sf::Keyboard::Return:
		case sf::Keyboard::Space:
			switch (m_choice)
			{
			case 0:
				wv::IApp::instance()->getStateManager().push(std::shared_ptr<wv::IState>(new OverworldState), wv::modality::EXCLUSIVE);
				break;
			case 1:
				wv::IApp::instance()->quit(0);
				break;
			}
			break;
		case sf::Keyboard::Escape:
			wv::IApp::instance()->quit(0);
		}
		return true;
	}
	return false;
}

void MainMenuState::render(sf::RenderTarget & rt)
{
	rt.setView(*m_view);
	rt.draw(*m_bg);
	rt.draw(*m_title);
	for (int i = 0; i < m_options.size(); i++)
	{
		if (i == m_choice)
		{
			m_options[i]->setColor(sf::Color::White);
		}
		else
		{
			m_options[i]->setColor(sf::Color(0, 0, 0, 150));
		}
		rt.draw(*m_options[i]);
	}
}
