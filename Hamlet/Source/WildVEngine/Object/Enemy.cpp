#include "Enemy.hpp"

namespace wv
{
	Enemy::Enemy(const std::string& name, TileMap * map, unsigned int r, unsigned int c, float speed, int health)
		:Battler(map, r, c, speed, health, sf::seconds(0.2f), false), m_name(name)
	{}

	Enemy::Enemy(const std::string & name, float speed, unsigned int health)
		:Battler(speed, health, sf::seconds(0.2f), false), m_name(name)
	{
	}

	Direction Enemy::track(Battler & b)
	{
		Direction d = Direction::NONE;
		auto pos = getCurrPosition();
		auto dest = b.getLastPosition();

		if (pos == dest)
		{
			return d;
		}
		float radians = std::atan2(dest.y - pos.y, dest.x - pos.x);
		float degrees = (radians * 180.f / 3.14159265f);

		if (degrees >= -67.5 && degrees <= 67.5)
		{
			d |= Direction::RIGHT;
		}
		else if (degrees <= -112.5 || degrees >= 112.5)
		{
			d |= Direction::LEFT;
		}

		if (degrees >= 22.5 && degrees <= 157.5)
		{
			d |= Direction::DOWN;
		}
		else if (degrees >= -157.5 && degrees <= -22.5)
		{
			d |= Direction::UP;
		}
		return d;
	}

	Enemy::~Enemy()
	{}
	const std::string & Enemy::getName() const
	{
		return m_name;
	}
}