#pragma once

#include "Battler.hpp"
#include <memory>

namespace wv
{
	class Enemy : public Battler
	{
		const std::string m_name;
	public:
		virtual ~Enemy();
		
		virtual void collided(Player& p) {};
		const std::string& getName() const;

		virtual std::shared_ptr<Enemy> spawn(TileMap* t, unsigned int r, unsigned int c) = 0;
	protected:
		Enemy(const std::string& name, TileMap* map, unsigned int r, unsigned int c, float speed, int health);
		Enemy(const std::string& name, float speed, unsigned int health);

		Direction track(Battler& b);
	};
}