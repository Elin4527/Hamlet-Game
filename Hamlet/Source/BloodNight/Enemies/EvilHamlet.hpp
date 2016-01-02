#pragma once
#include "../../WildVEngine/WildVEngine.hpp"

class EvilHamlet: public wv::Enemy
{
	wv::AbilitySet m_abilities;
	sf::Time m_pause;
	sf::Time m_cooldown;
	bool m_attacking;
public:
	EvilHamlet(wv::TileMap* t, unsigned int r, unsigned int c);
	EvilHamlet();
	virtual ~EvilHamlet();

	virtual void fixedUpdate() override;

	// Inherited via Enemy
	virtual std::shared_ptr<Enemy> spawn(wv::TileMap * t, unsigned int r, unsigned int c) override;
};