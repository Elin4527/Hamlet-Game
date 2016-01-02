#include "Laertes.hpp"
#include "../Abilities/LSlash.hpp"
#include "../Abilities/RSlash.hpp"
#include "../Abilities/Thrust.hpp"

Laertes::Laertes(wv::TileMap* t, unsigned int r, unsigned int c)
	:wv::Enemy("Laertes", t, r, c, 4.0f, 100), m_pause(sf::seconds(0.5f))
{
	loadFromFile("Data/Characters/laertes.txt");
	m_abilities.addAbility(&LSlash(sf::seconds(6.0f)));
	m_abilities.addAbility(&RSlash(sf::seconds(6.0f)));
	m_abilities.addAbility(&LSlash(sf::seconds(6.0f)));
	m_abilities.addAbility(&RSlash(sf::seconds(6.0f)));
	m_abilities.setPlayer(*this);
	setAbilities(m_abilities);
}

Laertes::Laertes()
	: wv::Enemy("Laertes", 4.0f, 100)
{
	loadFromFile("Data/Characters/laertes.txt");
	m_abilities.addAbility(&LSlash(sf::seconds(6.0f)));
	m_abilities.addAbility(&RSlash(sf::seconds(6.0f)));
	m_abilities.addAbility(&LSlash(sf::seconds(6.0f)));
	m_abilities.addAbility(&RSlash(sf::seconds(6.0f)));
	m_abilities.setPlayer(*this);
	setAbilities(m_abilities);
}

Laertes::~Laertes()
{
}

void Laertes::fixedUpdate()
{
	if (!getStopped())
	{
		m_pause -= wv::IApp::instance()->getTime().getTicksPerFrame();
		if (m_pause <= sf::Time::Zero)
		{
			for (int i = 0; i < getAbilities()->size(); i++)
			{
				if (getAbilities()->getAbility(i)->getRemainingTime() == sf::Time::Zero)
				{
					getAbilities()->useAbility(i);
					break;
				}
			}
			m_pause = sf::seconds(0.5f);
		}
	}
	auto hamlet = getTileMap()->getPlayer();
	setIntention(track(*hamlet));
	Enemy::fixedUpdate();
}


std::shared_ptr<wv::Enemy> Laertes::spawn(wv::TileMap * t, unsigned int r, unsigned int c)
{
	return std::make_shared<Laertes>(t, r, c);
}
