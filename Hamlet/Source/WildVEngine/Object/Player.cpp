#include "Player.hpp"
#include "../Abilities/AbilitySet.hpp"

namespace wv
{
	Player::Player(TileMap * map, unsigned int r, unsigned int c, float speed, unsigned int health,  unsigned int maxInv)
		:Battler(map, r, c, speed, health, sf::seconds(0.4f), true) , m_maxInventory(maxInv), m_textBox(nullptr)
	{
	}

	Player::~Player()
	{}

	TextBox * Player::getTextBox()
	{
		return m_textBox;
	}

	size_t Player::getMaxInventorySize() const
	{
		return m_maxInventory;
	}

	bool Player::handleInput(sf::Event & e)
	{
		if (e.type == sf::Event::KeyPressed)
		{
			switch (e.key.code)
			{
			case sf::Keyboard::Tab:
				if (!m_inventory.empty())
				{
					m_selectedItem++;
					if (m_selectedItem >= m_inventory.size())
					{
						m_selectedItem = 0;
					}
					if (m_inventory[m_selectedItem])
					{
						m_inventory[m_selectedItem]->use(*this);
					}
				}
				break;
			case sf::Keyboard::Q:
				if (getAbilities())
				{
					getAbilities()->useAbility(0);
				}
				break;
			case sf::Keyboard::W:
				if (getAbilities())
				{
					getAbilities()->useAbility(1);
				}
				break;
			case sf::Keyboard::E:
				if (getAbilities())
				{
					getAbilities()->useAbility(2);
				}
				break;	
			case sf::Keyboard::R:
				if (getAbilities())
				{
					getAbilities()->useAbility(3);
				}
				break;
			}
		}
		return false;
	}

	void Player::addItem( IItem* item)
	{
		if (m_inventory.size() < m_maxInventory && item)
		{
			m_inventory.push_back(item);
			if (m_inventory.size() == 1)
			{
				item->use(*this);
				m_selectedItem = 0;
			}
		}
	}

	void Player::setTextBox(TextBox * t)
	{
		m_textBox = t;
	}

	void Player::fixedUpdate()
	{
		if (m_textBox && m_textBox->isVisible())
		{
			setStopped(true);
		}
		else
		{
			setStopped(false);
		}

		updateIntention();
		Battler::fixedUpdate();
	}

	void Player::updateIntention()
	{
		Direction intention = Direction::NONE;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			intention |= Direction::UP;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			intention |= Direction::DOWN;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			intention |= Direction::LEFT;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			intention |= Direction::RIGHT;
		}

		setIntention(intention);
	}
}