#pragma once
#include "../../WildVEngine/WildVEngine.hpp"

class Polonius : public wv::Enemy
{
	wv::AbilitySet m_abilities;
	sf::Time m_spawnDelay;
public:
	Polonius(wv::TileMap* t, unsigned int r, unsigned int c);
	Polonius();
	virtual ~Polonius();

	virtual void fixedUpdate() override;

	// Inherited via Enemy
	virtual std::shared_ptr<Enemy> spawn(wv::TileMap * t, unsigned int r, unsigned int c) override;
};