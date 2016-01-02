#include "VictoryState.hpp"

void VictoryState::entered()
{
	m_view = new sf::View(sf::FloatRect(0, 0, wv::IApp::instance()->getWidth(), wv::IApp::instance()->getHeight()));
	m_background.setID("Images/Backgrounds/final.png");
	m_font.setID("Font/ARBERKLEY.ttf");

	// Create the background
	m_bg = new sf::Sprite(m_background.getResource());

	m_end = new sf::Text("The End. Thanks for Playing", m_font.getResource(), 30);
	m_end->setOrigin(std::roundf(m_end->getLocalBounds().width / 2), 0);
	m_end->setPosition(wv::IApp::instance()->getWidth() / 2, 150);
	m_end->setColor(sf::Color(255, 255, 100));

	// Initialize the textbox for message display
	m_textBox.reset(new wv::TextBox(nullptr, "Font/ARBERKLEY.ttf", "Images/GUI/textArea2.png", 20, 10, true));
	m_textBox->setLocation(sf::Vector2f(0.0f, 380.0f));
	m_textBox->setColor(sf::Color::White);
	m_textBox->loadFile("Scripts/end.txt");
}

void VictoryState::exiting()
{
	delete m_bg;
	delete m_end;

}

bool VictoryState::handleInput(sf::Event & e)
{
	if (m_textBox->isVisible())
	{
		return m_textBox->handleInput(e);
	}

	if (e.type == sf::Event::KeyPressed)
	{
		wv::IApp::instance()->getStateManager().pop();
		return true;
	}
	return false;
}

void VictoryState::fixedUpdate()
{
	m_textBox->fixedUpdate();
}

void VictoryState::render(sf::RenderTarget & rt)
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
		rt.draw(*m_end);
	}
}


