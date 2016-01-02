#pragma once

#include "IObject.hpp"
#include "Tile.hpp"
#include <vector>
#include <string>

namespace wv
{
	class Enemy;
	class Player;
	class IEvent;
	class MonsterSpawner;

	class TileMap : public virtual RenderComponent, public virtual LogicComponent
	{
		// Member variables
		
		std::shared_ptr<Player> m_player;
		std::vector<std::shared_ptr<Enemy>> m_units;
		std::vector<std::unique_ptr<IEvent>> m_events;
		std::vector<std::unique_ptr<DynamicGraphic>> m_effects;
		std::unique_ptr<MonsterSpawner> m_spawner;
		sf::Color m_tint;
		sf::RenderTexture m_viewable;
		sf::Sprite m_sprite;
		sf::Vector2u m_spawnPoint;
		std::vector<Tile*> m_tiles;
		std::vector<Tile*> m_tileSet;
		std::vector<bool> m_collisionMap;
		unsigned int m_rows;
		unsigned int m_cols;
		unsigned int m_tileSize;
		std::string m_mapChange;
		std::string m_loadedMap;

	public:
		TileMap();
		virtual ~TileMap();

		bool loadTileMap(const std::string& file);
		bool loadTileSet(const std::string& s);

		void queueMapChange(const std::string& map);

		Tile* getTile(unsigned int row, unsigned int col);
		Player* getPlayer();
		std::vector<std::shared_ptr<Enemy>> getEnemies(float start, float end);
		unsigned int getRows() const;
		unsigned int getColumns() const;
		unsigned int getTileSize() const;
		size_t getTotalEnemies() const;
		const sf::Vector2u& getSpawnPoint() const;
		const std::string& getLoadedMap() const;
		bool canMove(unsigned int row, unsigned int col);

		void setCollision(bool b, unsigned int r, unsigned int c);
		void addUnit(std::shared_ptr<Enemy> b);
		void addEvent(std::unique_ptr<IEvent> e);
		void addVisualEffect(std::shared_ptr<Animation> a, const sf::Vector2f& pos, const sf::Time& frameTime = sf::seconds(1.f / 12.f));
		void addVisualEffect(const std::string& str, const std::string& font, unsigned int ptSize, const sf::Vector2f& pos, sf::Time lifespan,
			float change, float radians, const sf::Color& start = sf::Color::Black, const sf::Color& end = sf::Color::Transparent);
		void addVisualEffect(const AnimatedSprite& a);
		void setPlayer(std::shared_ptr<Player> p);

		virtual void setColor(const sf::Color& c) override;

		// Inherited via RenderComponent
		virtual const sf::Drawable& getDrawable(const sf::View&) override;
		virtual void update() override;

		// Inherited via LogicComponent
		virtual void fixedUpdate() override;
	private:
		void changeMap(const std::string& file);

		std::vector<std::shared_ptr<Enemy>>::iterator lowerBoundSearch(float f);
		std::vector<std::shared_ptr<Enemy>>::iterator upperBoundSearch(float f);
	};
}