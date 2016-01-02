#include "ItemGlossary.hpp"
#include "../Items/IItem.hpp"

namespace wv
{
	ItemGlossary::ItemGlossary()
	{
	}
	ItemGlossary::~ItemGlossary()
	{
	}
	void ItemGlossary::addItem(std::unique_ptr<IItem> item)
	{
		if (item)
		{
			m_items[item->getName()] = std::move(item);
		}
	}
	IItem * ItemGlossary::getItem(const std::string & name)
	{
		auto iter = m_items.find(name);
		if (iter != m_items.end())
		{
			return m_items[name].get();
		}
		return nullptr;
	}
}