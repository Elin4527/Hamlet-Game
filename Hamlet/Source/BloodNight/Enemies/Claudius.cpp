#include "Claudius.hpp"
#include "../Abilities/LSlash.hpp"
#include "../Abilities/RSlash.hpp"
#include "../Abilities/Thrust.hpp"

Claudius::Claudius(wv::TileMap* t, unsigned int r, unsigned int c)
	:wv::Enemy("Claudius", t, r, c, 3.5f, 350), m_change(sf::seconds(5.0f)), m_pause(sf::Time::Zero),
	m_cooldown(sf::Time::Zero)
{
	loadFromFile("Data/Characters/claudius.txt");
	setColor(sf::Color::White);
	m_abilities.addAbility(&LSlash(sf::Time::Zero));
	m_abilities.addAbility(&RSlash(sf::Time::Zero));
	m_abilities.addAbility(&Thrust(sf::Time::Zero));
	m_abilities.setPlayer(*this);
	setAbilities(m_abilities);
}

Claudius::Claudius()
	:wv::Enemy("Claudius", 3.5f, 350), m_change(sf::seconds(5.0f)), m_pause(sf::Time::Zero)
{
	loadFromFile("Data/Characters/claudius.txt");
	setColor(sf::Color::White);
	m_abilities.addAbility(&LSlash(sf::Time::Zero));
	m_abilities.addAbility(&RSlash(sf::Time::Zero));
	m_abilities.addAbility(&Thrust(sf::Time::Zero));
	m_abilities.setPlayer(*this);
	setAbilities(m_abilities);
}

Claudius::~Claudius()
{
}

void Claudius::fixedUpdate()
{
	if (!getStopped())
	{
		m_pause -= wv::IApp::instance()->getTime().getTicksPerFrame();
		m_change -= wv::IApp::instance()->getTime().getTicksPerFrame();
		m_cooldown -= wv::IApp::instance()->getTime().getTicksPerFrame();

		if (m_change <= sf::Time::Zero)
		{
			m_change = sf::seconds(5.0f);
			m_pause = sf::Time::Zero;
			m_chasing = !m_chasing;
		}
		if (m_cooldown <= sf::Time::Zero && m_pause <= sf::Time::Zero && m_chasing)
		{

			getAbilities()->useAbility(m_currAbility);
			m_currAbility++;
			if (m_currAbility >= getAbilities()->size())
			{
				m_currAbility = 0;
				m_cooldown = sf::seconds(3.0f);
			}

			m_pause = sf::seconds(0.5f);
		}
	}
	auto hamlet = getTileMap()->getPlayer();

	if (m_chasing)
	{
		setIntention(track(*hamlet));
	}
	else
	{
		setIntention(wv::getOpposite(track(*hamlet)));
	}
	Enemy::fixedUpdate();
}


void Claudius::damage(unsigned int d)
{
	if (!m_chasing)
	{
		m_change = sf::seconds(5.0f);
		m_cooldown = sf::Time::Zero;
		m_pause = sf::Time::Zero;
		m_chasing = true;
	}
	Enemy::damage(d);
}

std::shared_ptr<wv::Enemy> Claudius::spawn(wv::TileMap * t, unsigned int r, unsigned int c)
{
	return std::make_shared<Claudius>(t, r, c);
}
