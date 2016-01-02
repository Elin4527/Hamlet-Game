#include "EquipItem.hpp"
#include "../Object/Player.hpp"

namespace wv {
	EquipItem::~EquipItem()
	{
	}

	void EquipItem::use(Player & p)
	{
		m_abilities.setPlayer(p);
		p.setAbilities(m_abilities);
	}

	EquipItem::EquipItem(const std::string & itemName, const std::string & texture)
		:IItem(itemName, texture)
	{
	}
	void EquipItem::addAbility(const IAbility* const a)
	{
		m_abilities.addAbility(a);
	}
}