#pragma once

#include <memory>
#include <map>

namespace wv
{
	class Enemy;

	// Holds a copy of every Enemy unit in the game for use with MonsterSpawner
	class MonsterIndex
	{
		std::map<const std::string, std::unique_ptr<Enemy>> m_enemies;
	public:
		MonsterIndex();
		virtual ~MonsterIndex();

		void addEnemy(std::unique_ptr<Enemy> enemy);
		Enemy* getEnemy(const std::string& name);
	};
};