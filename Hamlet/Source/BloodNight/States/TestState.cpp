#include "TestState.hpp"
#include <iostream>

void TestState::entered()
{
	 min = 300;
	 max = 350;
	int count = 0;
	m_view.reset(sf::FloatRect(0, 0, static_cast<float>(wv::IApp::instance()->getWidth()), static_cast<float>(wv::IApp::instance()->getHeight())));

	if (!m_tilemap.loadTileMap("Data\\room1.txt"))
	{
		std::cout << ("Map loading failed") << std::endl;
	}
	else
	{
		std::cout << "SUCCESS" << std::endl;
	}

	m_text = new wv::TextBox("Font\\AdobeGothicStd-Bold.otf", "Images\\textArea.png", 12, 10, true, false, false);
	m_text->setLocation(sf::Vector2f(0.0f, 380.f));
	m_text->setColor(sf::Color::Red);
	//m_text->loadString("I am Edwin and I am a coding monster. TROLL so lol exDee gale of the dankness. Such a beast lololol this is amazing. Programmer more like edwingrammer loooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooool");
	m_text->loadFile("Scripts\\test.txt");

	m_player = new wv::Player(&m_tilemap, 5, 5, 10);
	m_GUI = new wv::PlayerGUI(*m_player);

	unsigned int xOff = 0 * 32 * 3;
	unsigned int yOff = 0 * 32 * 4;

	std::string texture("Images/sprite3.png");
	std::shared_ptr<wv::Animation> anim = std::make_shared<wv::Animation>();
	anim->setSpriteSheet(texture);
	anim->addFrame(sf::IntRect(0 + xOff, 0 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(32 + xOff, 0 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(0 + xOff, 0 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(64 + xOff, 0 + yOff, 32, 32));
	m_player->addAnimation(wv::Direction::DOWN, anim);
	m_player->addAnimation(wv::Direction::DOWN | wv::Direction::LEFT, anim);

	anim = std::make_shared<wv::Animation>();
	anim->setSpriteSheet(texture);
	anim->addFrame(sf::IntRect(0 + xOff, 32 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(32 + xOff, 32 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(0 + xOff, 32 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(64 + xOff, 32 + yOff, 32, 32));
	m_player->addAnimation(wv::Direction::LEFT, anim);
	m_player->addAnimation(wv::Direction::UP | wv::Direction::LEFT, anim);


	anim = std::make_shared<wv::Animation>();	
	anim->setSpriteSheet(texture);
	anim->addFrame(sf::IntRect(0 + xOff, 64 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(32 + xOff, 64 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(0 + xOff, 64 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(64 + xOff, 64 + yOff, 32, 32));
	m_player->addAnimation(wv::Direction::RIGHT, anim);
	m_player->addAnimation(wv::Direction::DOWN | wv::Direction::RIGHT, anim);


	anim = std::make_shared<wv::Animation>();	
	anim->setSpriteSheet(texture);
	anim->addFrame(sf::IntRect(0 + xOff, 96 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(32 + xOff, 96 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(0 + xOff, 96 + yOff, 32, 32));
	anim->addFrame(sf::IntRect(64 + xOff, 96 + yOff, 32, 32));
	m_player->addAnimation(wv::Direction::UP, anim);
	m_player->addAnimation(wv::Direction::UP | wv::Direction::RIGHT, anim);

}

void TestState::fixedUpdate()
{
	if (m_text->isVisible()) 
	{
		m_text->fixedUpdate();
	}
	else
	{
		m_player->fixedUpdate();
	}
}

void TestState::update()
{
	m_tilemap.update();
	m_player->update();
}

void TestState::lateUpdate()
{
	m_view.setCenter(m_player->getCenter());
}

void TestState::render(sf::RenderTarget & rt)
{
	rt.setView(m_view);
	const sf::Drawable* t = &m_tilemap.getDrawable(m_view);
	rt.draw(*t, m_tilemap.getTransform());
	rt.draw(m_player->getDrawable(m_view), m_player->getTransform());
	t = &m_text->getDrawable(m_view);
	rt.draw(*t, m_text->getTransform());
	t = &m_GUI->getDrawable(m_view);
	rt.draw(*t, m_GUI->getTransform());
}
