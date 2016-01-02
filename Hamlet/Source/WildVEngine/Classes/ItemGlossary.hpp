#pragma once
#include <memory>
#include <map>
namespace wv
{
	class IItem;

	// Used to hold a copy of every item in the game
	class ItemGlossary
	{
		std::map<const std::string, std::unique_ptr<IItem>> m_items;
	public:
		ItemGlossary();
		virtual ~ItemGlossary();

		void addItem(std::unique_ptr<IItem> item);
		IItem* getItem(const std::string& name);
	};

}