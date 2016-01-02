#pragma once
#include "IObject.hpp"
#include "../Graphics2D/AnimatedSprite.hpp"
#include "TileMap.hpp"
#include "../Collision/THitbox.hpp"
#include <map>

namespace wv
{
	// Flag used for Facing Direction
	enum class Direction {
		NONE =	0,
		UP =	1 << 0,
		DOWN =	1 << 1,
		LEFT =	1 << 2,
		RIGHT = 1 << 3
	};

	inline Direction operator|(Direction a, Direction b)
	{
		return static_cast<Direction>(static_cast<int>(a) | static_cast<int>(b));
	}
	inline Direction operator&(Direction a, Direction b)
	{
		return static_cast<Direction>(static_cast<int>(a) & static_cast<int>(b));
	}
	inline Direction operator|=(Direction& a, Direction b)
	{
		return a = a | b;
	}
	inline Direction operator&=(Direction& a, Direction b)
	{
		return a = a | b;
	}

	inline Direction operator~(Direction a)
	{
		return static_cast<Direction>(~static_cast<unsigned int>(a) & 
			static_cast<unsigned int>(Direction::LEFT | Direction::RIGHT |Direction::UP | Direction::DOWN));
	}

	float convertToAngle(Direction D);
	Direction getOpposite(Direction D);

	class MapCharacter :public RenderComponent, public LogicComponent
	{
		static bool g_stopped;
		TileMap* m_map;
		Direction m_lastMove;
		Direction m_intention;
		sf::Vector2f m_velocity;
		sf::Vector2f m_lastPosition;
		sf::Vector2f m_currPosition;
		
		float m_speed;
		float m_angle;
		bool m_isMoving;
		bool m_friendly;

		// Hitbox Variables
		float m_cWidth;
		float m_cHeight;
		bool m_topLeft;
		bool m_topRight;
		bool m_botLeft;
		bool m_botRight;
		THitBox<sf::RectangleShape> m_hitBox;

		// Graphics variables
		AnimatedSprite m_sprite;
		std::map<Direction, std::shared_ptr<Animation>> m_animations;

	public:
		MapCharacter(TileMap* map, unsigned int r, unsigned int c, float speed, bool side);
		MapCharacter(float speed, bool side);
		virtual ~MapCharacter() = 0;

		virtual bool loadFromFile(const std::string& filename);
		void addAnimation(Direction d, std::shared_ptr<Animation> a);
		void setCollision(float width, float height);
		virtual void setColor(const sf::Color& c) override;
		void setTile(const sf::Vector2u& dest);
		void move(float magnitude, float angle);
		
		const sf::Vector2f& getLastPosition() const;
		const sf::Vector2f& getCurrPosition() const;
		bool getFriendly() const;
		float getAngle() const;
		float getCollisionWidth() const;
		float getCollisionHeight() const;

		sf::Vector2f getCenter() const;
		TileMap* getTileMap();

		bool contact(const MapCharacter& p);
		const THitBox<sf::RectangleShape>& getHitBox() const;

		virtual void fixedUpdate() override;
		virtual void update() override;
		virtual const sf::Drawable& getDrawable(const sf::View&) override;

		bool operator<(const MapCharacter& m);

		static bool getStopped();

		
	protected:
		void setIntention(Direction d);
		static void setStopped(bool b);

	private:
		void updatePos();
		void updateLogic();
		void updateHitbox();

		void calculateCorners(sf::Vector2f pos);

		void stopMoving();
		void startMoving(Direction d);
		void changeDirection(Direction d);
		sf::Vector2i getTile(sf::Vector2f pos);
	};

}