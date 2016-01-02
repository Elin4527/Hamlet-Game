#pragma once

#include "MapCharacter.hpp"
#include "../Items/IItem.hpp"

namespace wv
{
	class AbilitySet;

	class Battler : public MapCharacter
	{
		AbilitySet* m_abilities;
		unsigned int m_maxHealth;
		unsigned int m_health;
		sf::Time m_invulnerableTime;
		sf::Time m_timeRemaining;
		sf::Color m_color;
		bool m_nonAttackable;
	public:

		virtual ~Battler();

		unsigned int getMaxHealth() const;
		unsigned int getHealth() const;
		bool isDead() const;
		AbilitySet* getAbilities();

		virtual void damage(unsigned int d);
		void heal(unsigned int h);
		void setMaxHealth(unsigned int maxHealth, bool heal);
		void setNonAttackable(bool a);
		bool isAttackable();

		void setAbilities(AbilitySet& a);

		virtual void fixedUpdate() override;
		virtual void update() override;
		virtual void setColor(const sf::Color& color) override;
	protected:
		Battler(TileMap* map, unsigned int r, unsigned int c, float speed, unsigned int health,
			const sf::Time& invulnerability, bool friendly);
		Battler(float speed, unsigned int health, const sf::Time& invulnerability, bool friendly);

	};

}