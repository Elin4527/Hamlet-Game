
#pragma once
#include "../../WildVEngine/WildVEngine.hpp"

class Ghost : public wv::Enemy
{
public:
	Ghost(wv::TileMap* t, unsigned int r, unsigned int c);
	Ghost();
	virtual ~Ghost();

	virtual void fixedUpdate() override;
	virtual void collided(wv::Player& p) override;

	// Inherited via Enemy
	virtual std::shared_ptr<Enemy> spawn(wv::TileMap * t, unsigned int r, unsigned int c) override;
};