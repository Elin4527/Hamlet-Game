#include "AbilitySet.hpp"
#include "IAbility.hpp"

namespace wv
{

	AbilitySet::AbilitySet()
	{
	}

	AbilitySet::~AbilitySet()
	{
	}

	void AbilitySet::addAbility(const IAbility* const a)
	{
		if (a)
		{
			// When adding ability a copy is added to prevent the same instance being stored in multiple sets
			m_abilities.emplace_back(a->clone());
		}
		else
		{
			// Can also add a blank ability (used to fill up space on PlayerGUI)
			m_abilities.emplace_back();
		}
	}

	void AbilitySet::setPlayer(Battler & p)
	{
		m_player = &p;
	}

	// Updates cooldown of all abilities
	void AbilitySet::update(const sf::Time & t)
	{
		for (auto& a : m_abilities)
		{
			if (a)
			{
				a->update(t);
			}
		}
	}

	void AbilitySet::useAbility(std::size_t i)
	{
		if (i < m_abilities.size())
		{
			if (m_abilities[i] && m_abilities[i]->getRemainingTime() == sf::Time::Zero)
			{
				m_abilities[i]->use(*m_player);
			}
		}
	}

	IAbility* AbilitySet::getAbility(std::size_t i)
	{
		if (i < m_abilities.size())
		{
			return m_abilities[i].get();
		}
		return nullptr;
	}

	size_t AbilitySet::size() const
	{
		return m_abilities.size();
	}

}