#include "Battler.hpp"
#include "../Abilities/AbilitySet.hpp"

namespace wv
{

	Battler::~Battler()
	{
	}
	unsigned int Battler::getMaxHealth() const
	{
		return m_maxHealth;
	}

	unsigned int Battler::getHealth() const
	{
		return m_health;
	}

	bool Battler::isDead() const
	{
		return m_health == 0;
	}

	AbilitySet * Battler::getAbilities()
	{
		return m_abilities;
	}

	void Battler::damage(unsigned int d)
	{
		if (m_timeRemaining == sf::Time::Zero || !getFriendly())
		{
			if (d > m_health)
			{
				d = m_health;
			}

			m_health -= d;


			if (!getFriendly())
			{
				sf::Vector2f pos(std::roundf(getCurrPosition().x), std::roundf(getCurrPosition().y - 10.0f));
				getTileMap()->addVisualEffect(std::to_string(d), "Font/calibri.ttf", 12, pos, sf::seconds(2.0f), 5.f, convertToAngle(Direction::UP), sf::Color(255, 255, 255), sf::Color(255, 255, 255, 0));

			}
			m_timeRemaining = m_invulnerableTime;
		}
	}

	void Battler::heal(unsigned int h)
	{
		m_health += h;
		if (m_health > m_maxHealth)
		{
			m_health = m_maxHealth;
		}
	}

	void Battler::setMaxHealth(unsigned int maxHealth, bool heal)
	{
		if (heal && maxHealth > m_maxHealth)
		{
			m_health += maxHealth - m_maxHealth;
		}
		m_maxHealth = maxHealth;
		if (m_health > m_maxHealth)
		{
			m_health = m_maxHealth;
		}
	}

	void Battler::setNonAttackable(bool a)
	{
		m_nonAttackable = a;
	}

	bool Battler::isAttackable()
	{
		return !m_nonAttackable;
	}

	void Battler::setAbilities(AbilitySet & a)
	{
		m_abilities = &a;
	}

	void Battler::fixedUpdate()
	{
		m_timeRemaining -= wv::IApp::instance()->getTime().getTicksPerFrame();
		if (m_abilities)
		{
			m_abilities->update(wv::IApp::instance()->getTime().getTicksPerFrame());
		}
		if (m_timeRemaining < sf::Time::Zero)
		{
			m_timeRemaining = sf::Time::Zero;
		}
		MapCharacter::fixedUpdate();
	}

	void Battler::update()
	{
		MapCharacter::update();
		if (m_timeRemaining != sf::Time::Zero)
		{
			unsigned int gb = (static_cast<int>(m_timeRemaining / sf::seconds(0.2f)) % 2) * 100;
			MapCharacter::setColor(sf::Color(255, gb, gb, 255) * m_color);
		}
		else
		{
			MapCharacter::setColor(m_color);
		}
	}

	void Battler::setColor(const sf::Color & color)
	{
		m_color = color;
		MapCharacter::setColor(color);
	}

	Battler::Battler(TileMap * map, unsigned int r, unsigned int c, float speed, unsigned int health,
		const sf::Time& invulnerability, bool friendly)
		:MapCharacter(map, r, c, speed, friendly), m_maxHealth(health), m_health(health), m_color(sf::Color::White),
		m_invulnerableTime(invulnerability), m_timeRemaining(sf::Time::Zero), m_abilities(nullptr), m_nonAttackable(false)
	{
	}

	Battler::Battler(float speed, unsigned int health, const sf::Time & invulnerability, bool friendly)
		:MapCharacter(speed, friendly), m_maxHealth(health), m_health(health), m_color(sf::Color::White),
		m_invulnerableTime(invulnerability), m_timeRemaining(sf::Time::Zero), m_abilities(nullptr), m_nonAttackable(false)
	{
	}
}