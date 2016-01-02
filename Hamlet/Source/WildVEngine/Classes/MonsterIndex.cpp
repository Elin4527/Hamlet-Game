#include "MonsterIndex.hpp"
#include "../Object/Enemy.hpp"

namespace wv
{
	MonsterIndex::MonsterIndex()
	{
	}
	MonsterIndex::~MonsterIndex()
	{
	}
	void MonsterIndex::addEnemy(std::unique_ptr<Enemy> item)
	{
		if (item)
		{
			m_enemies[item->getName()] = std::move(item);
		}
	}
	Enemy * MonsterIndex::getEnemy(const std::string & name)
	{
		auto iter = m_enemies.find(name);
		if (iter != m_enemies.end())
		{
			return m_enemies[name].get();
		}
		return nullptr;
	}
}