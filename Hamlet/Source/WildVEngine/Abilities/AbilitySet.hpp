#pragma once

#include "IAbility.hpp"

namespace wv
{
	class IAbility;

	//	Used to store multiple abilities and be attached to a Battler
	class AbilitySet
	{
		std::vector<std::unique_ptr<IAbility>> m_abilities;
		Battler* m_player;	// Pointer to parent

	public:
		AbilitySet();
		virtual ~AbilitySet();

		void addAbility(const IAbility* ability);
		void setPlayer(Battler& p);
		void update(const sf::Time& t);
		void useAbility(std::size_t i);

		IAbility* getAbility(std::size_t i);
		size_t size() const;
	};
}