#pragma once
#include "../../WildVEngine/WildVEngine.hpp"

class Laertes : public wv::Enemy
{
	wv::AbilitySet m_abilities;
	sf::Time m_pause;
public:
	Laertes(wv::TileMap* t, unsigned int r, unsigned int c);
	Laertes();
	virtual ~Laertes();
	virtual void fixedUpdate() override;

	// Inherited via Enemy
	virtual std::shared_ptr<Enemy> spawn(wv::TileMap * t, unsigned int r, unsigned int c) override;
};