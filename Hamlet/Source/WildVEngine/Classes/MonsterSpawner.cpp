#include "MonsterSpawner.hpp"
#include "../Object/Player.hpp"
#include "../Object/Enemy.hpp"

namespace wv
{
	MonsterSpawner::MonsterSpawner()
		:m_map(nullptr), m_spawned(0)
	{

	}

	MonsterSpawner::~MonsterSpawner()
	{
	}

	void MonsterSpawner::setTileMap(TileMap * map)
	{
		m_map = map;
		for (unsigned int i = 0; i < m_map->getRows(); i++)
		{
			for (unsigned int j = 0; j < m_map->getColumns(); j++)
			{
				// Add all passable tiles to list of spawnable tiles
				if (m_map->canMove(i, j))
				{
					m_spawnable.push_back(i * m_map->getColumns() + j);
				}
			}
		}
	}

	void MonsterSpawner::addEnemySpawn(const std::string & name)
	{
		m_list.push_back(IApp::instance()->getMonsterIndex().getEnemy(name));
	}

	void MonsterSpawner::setSpawnTime(const sf::Time & time)
	{
		m_spawnTime = m_remainingTime = time;
	}

	void MonsterSpawner::setMultipleSpawn(unsigned int num)
	{
		m_numToSpawn = num;
	}

	void MonsterSpawner::setSpawnCount(unsigned int spawnCount)
	{
		m_spawnCount = spawnCount;
	}

	void MonsterSpawner::setMaximumEnemies(unsigned int maxEnemies)
	{
		m_maxEnemies = maxEnemies;
	}

	void MonsterSpawner::setScript(const std::string & script)
	{
		m_script = script;
	}

	void MonsterSpawner::setColor(const sf::Color & c)
	{
	}

	void MonsterSpawner::fixedUpdate()
	{
		// As long as a player exists and the characters are not paused
		if (!m_map->getPlayer() || !m_map->getPlayer()->getStopped())
		{
			// Decrease remaining time
			m_remainingTime -= IApp::instance()->getTime().getTicksPerFrame();

			if (m_remainingTime < sf::Time::Zero)
			{
				m_remainingTime = sf::Time::Zero;
			}

			// Spawn enemy if off colldown, there are still enemies left to spawn, and the map is not full
			if (m_remainingTime == sf::Time::Zero && m_spawned < m_spawnCount && m_map->getTotalEnemies() < m_maxEnemies)
			{
				m_remainingTime = m_spawnTime;

				// Spawn as many enemies as allowed in one spawn
				for (unsigned int i = 0; i < m_numToSpawn && m_spawned < m_spawnCount && m_map->getTotalEnemies() < m_maxEnemies; i++)
				{
					spawnEnemy();
				}
			}
		}

		// When finished load the script into the textbox and play it
		if (m_spawned == m_spawnCount && m_map->getTotalEnemies() == 0 && m_script != "")
		{
			m_map->getPlayer()->getTextBox()->loadFile(m_script);
			m_map->getPlayer()->getTextBox()->show();

			// Delete the script to prevent it from being continuously run
			m_script = "";
		}
	}

	void MonsterSpawner::spawnEnemy()
	{
		if (!m_list.empty())	// Check that there are enemies to spawn this level
		{
			unsigned int row, col;
			auto dim = m_map->getPlayer()->getCurrPosition();	// Store the players position
			
			std::srand(static_cast<unsigned int>(std::time(NULL)));	// seed the random number generator
			Enemy* e = m_list[rand() % m_list.size()];	// Choose a random valid enemy
			
			if (e)
			{
				do
				{
					size_t index = rand() % m_spawnable.size();	// Choose a random spawnable tile

					// Calculate row and column indices
					row = m_spawnable[index] / m_map->getColumns();
					col = m_spawnable[index] - row * m_map->getColumns();

					// Store distance from player as a vector
					dim = sf::Vector2f(m_map->getPlayer()->getCurrPosition().x - (col + .5f) * m_map->getTileSize(),	// x-distance
						m_map->getPlayer()->getCurrPosition().y - (row + .5f) * m_map->getTileSize());					// y-distance
				} while ((dim.x * dim.x + dim.y * dim.y) < 6400.f);	// While less than 5 blocks away
				
				// Add a unit to the map
				m_map->addUnit(e->spawn(m_map, row, col));
				m_spawned++;
			}
		}
	}
}