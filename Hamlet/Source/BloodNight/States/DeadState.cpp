#include "DeadState.hpp"

void DeadState::entered()
{
	m_view = new sf::View(sf::FloatRect(0, 0, wv::IApp::instance()->getWidth(), wv::IApp::instance()->getHeight()));
	m_background.setID("Images/Backgrounds/deathScreen.png");
	m_font.setID("Font/ARBERKLEY.ttf");

	// Create the background
	m_bg = new sf::Sprite(m_background.getResource());

	// Create the choices
	m_options.push_back(new sf::Text("Ay, theres the rub [Continue]", m_font.getResource(), 20));
	m_options[0]->setOrigin(std::roundf(m_options[0]->getLocalBounds().width / 2), 0);
	m_options[0]->setPosition(std::roundf(wv::IApp::instance()->getWidth() / 2), 200);
	m_options.push_back(new sf::Text("The rest is silence. [Quit]", m_font.getResource(), 20));
	m_options[1]->setOrigin(std::roundf(m_options[1]->getLocalBounds().width / 2), 0);
	m_options[1]->setPosition(std::roundf(wv::IApp::instance()->getWidth() / 2), 240);

	// Initialize the textbox for message display
	m_textBox.reset(new wv::TextBox(nullptr, "Font/ARBERKLEY.ttf", "Images/GUI/textArea2.png", 20, 10, true));
	m_textBox->setLocation(sf::Vector2f(0.0f, 0.0f));
	m_textBox->setColor(sf::Color::White);
	m_textBox->loadFile("Scripts/death.txt");

}

void DeadState::exiting()
{
	delete m_bg;
	for (auto o : m_options)
	{
		delete o;
	}
}

bool DeadState::handleInput(sf::Event & e)
{
	if (m_textBox->isVisible())
	{
		return m_textBox->handleInput(e);
	}

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
				finish();
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

void DeadState::fixedUpdate()
{
	m_textBox->fixedUpdate();
}

void DeadState::render(sf::RenderTarget & rt)
{
	rt.setView(*m_view);
	rt.draw(*m_bg);
	if (m_textBox->isVisible())
	{
		auto& d = m_textBox->getDrawable(*m_view);
		auto& t = m_textBox->getTransform();
		rt.draw(d, t);
	}
	else
	{
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
}


