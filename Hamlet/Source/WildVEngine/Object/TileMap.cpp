#include "TileMap.hpp"
#include <fstream>
#include <iostream>
#include "Player.hpp"
#include "Enemy.hpp"
#include "../Graphics2D/Animation.hpp"
#include "../Graphics2D/TextEffect.hpp"
#include "../Event/PickupEvent.hpp"
#include "../Event/MapChangeEvent.hpp"
#include "../Event/TextEvent.hpp"
#include "../Classes/MonsterSpawner.hpp"

namespace wv
{
	TileMap::TileMap()
		:m_tint(255,255,255), m_loadedMap()
	{
		m_viewable.create(640, 480);
		m_sprite.setTexture(m_viewable.getTexture());
	}

	TileMap::~TileMap()
	{
		for (auto& t : m_tiles)
		{
			delete t;
		}
		for (auto& t : m_tileSet)
		{
			delete t;
		}
	}

	bool TileMap::loadTileMap(const std::string& file)
	{
		std::ifstream in;
		in.open(file, std::ios::in);

		if (!in.is_open())
		{
			return false;
		}
		in >> m_rows >> m_cols >> m_tileSize;
		in >> m_spawnPoint.x >> m_spawnPoint.y;

		// Load Tileset
		std::string tileset;
		in >> tileset;
		if (!loadTileSet(tileset))
		{
			return false;
		}

		// Load Tiles
		for (unsigned int i = 0; i < m_rows; i++)
		{
			for (unsigned int j = 0; j < m_cols; j++)
			{
				int tileId;
				in >> tileId;
				Tile* t;
				t = m_tileSet[tileId]->clone();
				t->setPosition(static_cast<float>(j*m_tileSize), static_cast<float>(i*m_tileSize));
				m_tiles.push_back(t);
				m_collisionMap.push_back(true);
			}
		}
		
		// Load Mob Spawner
		m_spawner.reset(new MonsterSpawner());
		float spawnTime;
		in >> spawnTime;
		m_spawner->setTileMap(this);
		m_spawner->setSpawnTime(sf::seconds(spawnTime));

		unsigned int multi, count, max;
		in >> multi >> count >> max;
		m_spawner->setMultipleSpawn(multi);
		m_spawner->setSpawnCount(count);
		m_spawner->setMaximumEnemies(max);
		
		unsigned int enemies;
		in >> enemies;
		for (int i = 0; i < enemies; i++)
		{
			std::string name;
			in >> name;
			if (name != "NULL")
			{
				m_spawner->addEnemySpawn(name);
			}
			else
			{
				m_spawner->addEnemySpawn("");
			}
		}
		std::string finishScript;
		in >> finishScript;
		if (finishScript != "NULL")
		{
			m_spawner->setScript(finishScript);
		}

		// Load events
		int events;
		in >> events;

		for (int i = 0; i < events; i++)
		{
			int x, y;
			in >> x >> y;
			char type;
			in >> type;
			switch (type)
			{
			case 'P':
			{
				std::string item, script;
				in >> item >> script;
				addEvent(std::make_unique<PickUpEvent>(sf::Vector2f((x + .5f) * m_tileSize, (y + .5f) * m_tileSize), item, script));
				break;
			}
			case 'M':
			{
				std::string map;
				in >> map;
				addEvent(std::make_unique<MapChangeEvent>(sf::Vector2f((x + .5f) * m_tileSize, (y + .5f) * m_tileSize), map));
				break;
			}
			case 'T':
			{
				std::string script;
				in >> script;
				addEvent(std::make_unique<TextEvent>(sf::Vector2f((x + .5f) * m_tileSize, (y + .5f) * m_tileSize), script));
				break;
			}
			}
		}

		m_loadedMap = file;
		return true;
	}

	bool TileMap::loadTileSet(const std::string& file)
	{
		std::ifstream in;
		in.open(file, std::ios::in);
		if (!in.is_open())
		{
			return false;
		}
		std::string image;
		in >> image;


		int frames;
		in >> frames;
		while (!in.eof())
		{
			Animation* anim = new Animation;
			anim->setSpriteSheet(image);

			for (int i = 0; i < frames; i++)
			{
				int x, y;
				in >> x >> y;

				anim->addFrame(sf::IntRect(x, y, m_tileSize, m_tileSize));
			}

			bool pass;
			in >> pass;

			m_tileSet.push_back(new Tile(std::shared_ptr<Animation>(anim), pass));
			in >> frames;
		}

		return true;
	}

	void TileMap::queueMapChange(const std::string & map)
	{
		m_mapChange = map;
	}

	void TileMap::changeMap(const std::string & file)
	{
		using std::swap;
		TileMap next;
		next.loadTileMap(file);
		
		swap(m_units, next.m_units);
		swap(m_events, next.m_events);
		swap(m_effects, next.m_effects);
		swap(m_tint, next.m_tint);
		swap(m_spawnPoint, next.m_spawnPoint);
		swap(m_tiles, next.m_tiles);
		swap(m_tileSet, next.m_tileSet);
		swap(m_collisionMap, next.m_collisionMap);
		swap(m_rows, next.m_rows);
		swap(m_cols, next.m_cols);
		swap(m_tileSize, next.m_tileSize);
		swap(m_loadedMap, next.m_loadedMap);
		m_spawner = std::move(next.m_spawner);
		m_spawner->setTileMap(this);
		m_player->setTile(m_spawnPoint);
	}

	Tile * TileMap::getTile(unsigned int r, unsigned int c)
	{
		if (r >= m_rows || c >= m_cols)
		{
			return nullptr;
		}
		return m_tiles[r * m_cols + c];
	}

	Player * TileMap::getPlayer()
	{
		return m_player.get();
	}

	std::vector<std::shared_ptr<Enemy>> TileMap::getEnemies(float start, float end)
	{
		return std::vector<std::shared_ptr<Enemy>>(lowerBoundSearch(start), upperBoundSearch(end));
	}

	unsigned int TileMap::getRows() const
	{
		return m_rows;
	}

	unsigned int TileMap::getColumns() const
	{
		return m_cols;
	}

	unsigned int TileMap::getTileSize() const
	{
		return m_tileSize;
	}

	size_t TileMap::getTotalEnemies() const
	{
		return m_units.size();
	}

	const sf::Vector2u & TileMap::getSpawnPoint() const
	{
		return m_spawnPoint;
	}

	const std::string & TileMap::getLoadedMap() const
	{
		return m_loadedMap;
	}

	bool TileMap::canMove(unsigned int r, unsigned int c)
	{
		auto t = getTile(r, c);
		if (!t)
			return false;
		return t->isPassable() && m_collisionMap[r * m_cols + c];
	}

	void TileMap::setCollision(bool b, unsigned int r, unsigned int c)
	{
		if (r >= m_rows || c >= m_cols)
			return;
		m_collisionMap[r * m_cols + c] = b;
	}

	void TileMap::addUnit(std::shared_ptr<Enemy> b)
	{
		if (b)
		{
			m_units.push_back(b);
		}
	}

	void TileMap::addEvent(std::unique_ptr<IEvent> e)
	{
		m_events.push_back(std::move(e));
	}

	void TileMap::addVisualEffect(std::shared_ptr<Animation> a, const sf::Vector2f & pos, const sf::Time& frameTime)
	{
		std::unique_ptr<AnimatedSprite> e(std::make_unique<AnimatedSprite>( frameTime, false, false));
		e->setAnimation(a);
		e->setOrigin(e->getLocalBounds().width / 2, e->getLocalBounds().height / 2);
		e->setPosition(pos);
		m_effects.push_back(std::move(e));
	}

	void TileMap::addVisualEffect(const std::string & str, const std::string & font, unsigned int ptSize, const sf::Vector2f & pos, sf::Time lifespan, float change, float radians, const sf::Color & start, const sf::Color & end)
	{
		m_effects.emplace_back(std::make_unique<TextEffect>(str, font, ptSize, pos, lifespan, change, radians, start, end));
	}

	void TileMap::addVisualEffect(const AnimatedSprite & a)
	{
		m_effects.emplace_back(std::make_unique<AnimatedSprite>(a));
	}

	void TileMap::setPlayer(std::shared_ptr<Player> p)
	{
		m_player = p;
	}

	void TileMap::setColor(const sf::Color & c)
	{
		m_tint = c;
	}

	const sf::Drawable& wv::TileMap::getDrawable(const sf::View& v)
	{
		sf::Vector2f dim = v.getSize();
		sf::Vector2f center = v.getCenter();
		sf::FloatRect screenRect(sf::Vector2f(center.x - (dim.x / 2.0f), center.y - (dim.y) / 2), dim);
		m_viewable.clear(sf::Color::Transparent);

		setPosition(screenRect.left, screenRect.top);

		m_viewable.setView(v);

		int startX = screenRect.top / m_tileSize;
		startX = std::max(startX, 0);
		int endX = (screenRect.top + screenRect.height) / m_tileSize + 1;
		endX = std::min(endX, static_cast<int>(m_rows));
		int startY = screenRect.left / m_tileSize;
		startY = std::max(startY, 0);
		int endY = (screenRect.left + screenRect.width) / m_tileSize + 1;
		endY = std::min(endY, static_cast<int>(m_cols));

		for (int i = startX; i < endX; i++)
		{
			for (int j = startY; j < endY; j++)
			{
				Tile* t = m_tiles[i * m_cols + j];
				t->setColor(m_tint);
				m_viewable.draw(t->getDrawable(v), t->getTransform());
			}
		}

		for (auto& e : m_events)
		{
			auto& d = e->getDrawable(v);
			auto& t = e->getTransform();
			m_viewable.draw(d, t);
		}

		for (auto u : m_units)
		{
			if (u->getHitBox().isVisible())
			{
				m_viewable.draw(u->getHitBox());
			}
			auto& d = u->getDrawable(v);
			auto& t = u->getTransform();
			m_viewable.draw(d, t);
		}

		if (m_player)
		{
			if (m_player->getHitBox().isVisible())
			{
				m_viewable.draw(m_player->getHitBox());
			}
			auto& d = m_player->getDrawable(v);
			auto& t = m_player->getTransform();
			m_viewable.draw(d, t);
		}
		for (auto& e : m_effects)
		{
			m_viewable.draw(*e);
		}

		m_viewable.display();

		return m_sprite;
	}

	void TileMap::update()
	{
		for (auto& e : m_events)
		{
			e->update();
		}
		for (auto p : m_tiles)
		{
			p->update();
		}
		for (auto u : m_units)
		{
			u->update();
		}
		if (m_player)
		{
			m_player->update();
		}
		sf::Time t = IApp::instance()->getTime().getDeltaRenderTime();
		auto iter = m_effects.begin();
		while (iter != m_effects.end())
		{
			(*iter)->update(t);

			if ((*iter)->isPlaying())
			{
				iter++;
			}
			else
			{
				iter = m_effects.erase(iter);
			}
		}
	}

	void TileMap::fixedUpdate()
	{
		if (m_mapChange != "")
		{
			changeMap(m_mapChange);
			m_mapChange = "";
		}
		auto iter = m_units.begin();
		while(iter != m_units.end())
		{
			if (!(*iter) || (*iter)->isDead())
			{
				iter = m_units.erase(iter);
			}
			else
			{
				(*iter)->fixedUpdate();
				iter++;
			}
		}
		auto e = m_events.begin();
		while (e != m_events.end())
		{
			if (!(*e) || !(*e)->isActive())
			{
				e = m_events.erase(e);
			}
			else
			{
				e++;
			}
		}
		std::sort(m_units.begin(), m_units.end(), 
			[](std::shared_ptr<Enemy> a, std::shared_ptr<Enemy> b) {
			return *a < *b; 
		});
		if (m_player)
		{
			m_player->fixedUpdate();
			if (!MapCharacter::getStopped())
			{
				auto dim = m_player->getHitBox().getGlobalBounds();

				auto iter = lowerBoundSearch(dim.left);
				auto end = upperBoundSearch(dim.left + dim.width);
				for (; iter != end; iter++)
				{
					if (m_player->contact(**iter))
					{
						(*iter)->collided(*m_player);
					}
				}


				for (auto it = m_events.begin(); it < m_events.end(); it++)
				{
					if (m_player->getHitBox().collision((*it)->getHitBox()))
					{
						(*it)->collided(*m_player);
					}
				}
			}
		}

		m_spawner->fixedUpdate();
	}

	std::vector<std::shared_ptr<Enemy>>::iterator TileMap::lowerBoundSearch(float f)
	{
		auto iter = m_units.begin();
		auto first = iter;
		auto count = m_units.end() - m_units.begin();
		auto step = count;
		while (count > 0)
		{
			iter = first;
			step = count / 2;
			std::advance(iter, step);
			if((*iter)->getCurrPosition().x + ((*iter)->getCollisionWidth() / 2) < f)
			{
				first = ++iter;
				count -= step + 1;
			}
			else
			{
				count = step;
			}
		}
		return first;
	}

	std::vector<std::shared_ptr<Enemy>>::iterator TileMap::upperBoundSearch(float f)
	{
		auto iter = m_units.begin();
		auto first = iter;
		auto count = m_units.end() - m_units.begin();
		auto step = count;
		while (count > 0)
		{
			iter = first;
			step = count / 2;
			std::advance(iter, step);
			if ((*iter)->getCurrPosition().x - ((*iter)->getCollisionWidth() / 2) > f)
			{
				count = step;
			}
			else
			{
				first = ++iter;
				count -= step + 1;
			}
		}
		return first;
	}

}
