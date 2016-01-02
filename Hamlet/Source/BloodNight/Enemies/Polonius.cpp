#include "Polonius.hpp"
#include "../Abilities/Punch.hpp"

Polonius::Polonius(wv::TileMap* t, unsigned int r, unsigned int c)
	:wv::Enemy("Polonius", t, r, c, 3.0f, 40), m_spawnDelay(sf::seconds(1.0f))
{
	loadFromFile("Data/Characters/polonius.txt");
	m_abilities.addAbility(&Punch(sf::seconds(5.0f)));
	m_abilities.setPlayer(*this);
	setAbilities(m_abilities);
}

Polonius::Polonius()
	:wv::Enemy("Polonius", 3.0f, 40), m_spawnDelay(sf::seconds(1.0f))
{
	loadFromFile("Data/Characters/polonius.txt");
	m_abilities.addAbility(&Punch(sf::seconds(5.0f)));
	m_abilities.setPlayer(*this);
	setAbilities(m_abilities);
}

Polonius::~Polonius()
{
}

void Polonius::fixedUpdate()
{
	m_spawnDelay -= wv::IApp::instance()->getTime().getTicksPerFrame();
	if (m_spawnDelay <= sf::Time::Zero)
	{
		for (int i = 0; i < getAbilities()->size(); i++)
		{
			if (getAbilities()->getAbility(i)->getRemainingTime() == sf::Time::Zero)
			{
				getAbilities()->useAbility(i);
			}
		}
	}
	auto hamlet = getTileMap()->getPlayer();
	setIntention(track(*hamlet));
	Enemy::fixedUpdate();
}

std::shared_ptr<wv::Enemy> Polonius::spawn(wv::TileMap * t, unsigned int r, unsigned int c)
{
	return std::make_shared<Polonius>(t, r, c);
}
