#pragma once
#include "IItem.hpp"
#include "../Abilities/AbilitySet.hpp"

namespace wv
{
	// Item that gives the player abilities when equipped (use function)
	class EquipItem : public IItem
	{
		AbilitySet m_abilities;
	public:
		virtual ~EquipItem();
		virtual void use(Player& p) override;

	protected:
		EquipItem(const std::string& itemName, const std::string& texture);
		void addAbility(const IAbility* const a);
	};

}