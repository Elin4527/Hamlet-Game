#pragma once
#include "../Object/TileMap.hpp"
#include "../Object/IObject.hpp"

namespace wv
{
	class Enemy;

	// Spawns enemys according to certain parameters and stores a script to be executed at the end
	// of the level after all enemies are defeated
	class MonsterSpawner: public LogicComponent
	{
		std::vector<Enemy*> m_list;
		std::vector<unsigned int> m_spawnable;		// List of tiles that can be used for spawning
		TileMap* m_map;
		sf::Time m_spawnTime;
		sf::Time m_remainingTime;
		unsigned int m_spawned;
		unsigned int m_spawnCount;
		unsigned int m_numToSpawn;
		unsigned int m_maxEnemies;
		std::string m_script;
	public:

		MonsterSpawner();
		virtual ~MonsterSpawner();

		void setTileMap(TileMap* map);
		void addEnemySpawn(const std::string& name);
		void setSpawnTime(const sf::Time& time);
		void setMultipleSpawn(unsigned int num);
		void setSpawnCount(unsigned int spawnCount);
		void setMaximumEnemies(unsigned int maxEnemies);
		void setScript(const std::string& script);

		virtual void setColor(const sf::Color & c) override;
		virtual void fixedUpdate() override;
		
	private:
		void spawnEnemy();

	};

}