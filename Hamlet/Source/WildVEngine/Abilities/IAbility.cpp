#include "IAbility.hpp"

namespace wv
{
	IAbility::~IAbility()
	{}

	// Call this version first when overriding
	void IAbility::use(Battler & p)
	{
		// Only usable if off cooldown
		if (m_remainingCD != sf::Time::Zero)
		{
			return;
		}

		// Set cooldown after usage
		m_remainingCD = m_coolDown;
	}

	// Updates the cooldown of the ability each logic frame
	void IAbility::update(const sf::Time & t)
	{
		m_remainingCD -= t;
		if (m_remainingCD < sf::Time::Zero)
		{
			m_remainingCD = sf::Time::Zero;
		}
	}

	sf::Texture & IAbility::getTexture()
	{
		return m_icon.getResource();
	}

	const std::string & IAbility::getName() const
	{
		return m_name;
	}

	const sf::Time & IAbility::getRemainingTime() const
	{
		return m_remainingCD;
	}

	const sf::Time & IAbility::getCoolDown() const
	{
		return m_coolDown;
	}

	IAbility::IAbility(const std::string & name, const std::string& texture, const sf::Time& cooldown)
		:m_name(name), m_icon(texture), m_coolDown(cooldown), m_remainingCD(sf::Time::Zero)
	{
	}

	// Default no effect
	void IAbility::effect(Battler & user, Battler & target)
	{
	}
}