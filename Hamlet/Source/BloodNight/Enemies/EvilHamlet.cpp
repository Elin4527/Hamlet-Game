#include "EvilHamlet.hpp"
#include "../Abilities/Punch.hpp"

EvilHamlet::EvilHamlet(wv::TileMap* t, unsigned int r, unsigned int c)
	:wv::Enemy("Hamlet", t, r, c, 2.0f, 10), m_pause(sf::Time::Zero), m_cooldown(sf::seconds(1.0f))
{
	loadFromFile("Data/Characters/hamlet.txt");
	setColor(sf::Color(100, 100, 100, 255));
	m_abilities.addAbility(&Punch(sf::seconds(0.0f)));
	m_abilities.setPlayer(*this);
	setAbilities(m_abilities);
}

EvilHamlet::EvilHamlet()
	:wv::Enemy("Hamlet", 2.0f, 10), m_pause(sf::Time::Zero), m_cooldown(sf::seconds(1.0f))
{
	loadFromFile("Data/Characters/hamlet.txt");
	setColor(sf::Color(100, 100, 100, 255));
	m_abilities.addAbility(&Punch(sf::seconds(0.0f)));
	m_abilities.setPlayer(*this);
	setAbilities(m_abilities);
}

EvilHamlet::~EvilHamlet()
{
}

void EvilHamlet::fixedUpdate()
{
	m_cooldown -= wv::IApp::instance()->getTime().getTicksPerFrame();

	if (m_cooldown <= sf::Time::Zero && !m_attacking)
	{
		m_pause = sf::seconds(1.0f);
		m_attacking = true;
	}

	m_pause -= wv::IApp::instance()->getTime().getTicksPerFrame();
	if (m_pause <= sf::Time::Zero && m_cooldown <= sf::Time::Zero && m_attacking)
	{
		for (int i = 0; i < getAbilities()->size(); i++)
		{
			if (getAbilities()->getAbility(i)->getRemainingTime() == sf::Time::Zero)
			{
				getAbilities()->useAbility(i);
			}
		}
		m_cooldown = sf::seconds(4.0f);
		m_attacking = false;
	}

	if (m_pause > sf::Time::Zero)
	{
		setIntention(wv::Direction::NONE);
	}
	else
	{
		auto hamlet = getTileMap()->getPlayer();
		setIntention(track(*hamlet));
	}
	Enemy::fixedUpdate();
}


std::shared_ptr<wv::Enemy> EvilHamlet::spawn(wv::TileMap * t, unsigned int r, unsigned int c)
{
	return std::make_shared<EvilHamlet>(t, r, c);
}
