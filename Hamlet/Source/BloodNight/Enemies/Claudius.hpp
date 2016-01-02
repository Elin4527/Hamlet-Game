#pragma once
#include "../../WildVEngine/WildVEngine.hpp"

class Claudius : public wv::Enemy
{
	sf::Time m_change;
	bool m_chasing;
	wv::AbilitySet m_abilities;
	sf::Time m_pause;
	sf::Time m_cooldown;
	unsigned int m_currAbility;
public:
	Claudius(wv::TileMap* t, unsigned int r, unsigned int c);
	Claudius();
	virtual ~Claudius();

	virtual void fixedUpdate() override;
	virtual void damage(unsigned int d) override;

	// Inherited via Enemy
	virtual std::shared_ptr<Enemy> spawn(wv::TileMap * t, unsigned int r, unsigned int c) override;
};