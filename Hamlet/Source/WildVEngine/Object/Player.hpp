#pragma once

#include "Battler.hpp"
#include "../Items/IItem.hpp"
#include "TextBox.hpp"

namespace wv
{
	class AbilitySet;

	class Player : public Battler
	{
		std::vector<IItem*> m_inventory;
		size_t m_maxInventory;
		size_t m_selectedItem;
		TextBox* m_textBox;

	public:
		friend class PlayerGUI;

		Player(TileMap* map, unsigned int r, unsigned int c, float speed, unsigned  int health = 5, unsigned int maxInventory = 5);
		virtual ~Player();

		TextBox* getTextBox();
		size_t getMaxInventorySize() const;

		virtual bool handleInput(sf::Event& e) override;
		void addItem(IItem* item);
		void setTextBox(TextBox* t);

		virtual void fixedUpdate() override;
	private:
		void updateIntention();
	};

}