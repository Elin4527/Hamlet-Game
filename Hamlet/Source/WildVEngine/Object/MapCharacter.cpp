#include "MapCharacter.hpp"
#include <iostream>
#include <fstream>
#include <memory>

namespace wv
{
	float convertToAngle(Direction d)
	{
		int x = 0;
		if (static_cast<int>(d & Direction::RIGHT))
		{
			x = 1;
		}
		else if (static_cast<int>(d & Direction::LEFT))
		{
			x = -1;
		}
		int y = 0;
		if (static_cast<int>(d & Direction::DOWN))
		{
			y = 1;
		}
		else if (static_cast<int>(d & Direction::UP))
		{
			y = -1;
		}
		return std::atan2(y, x);
	}

	Direction getOpposite(Direction D)
	{
		Direction ret = Direction::NONE;

		if (static_cast<unsigned int>(D & Direction::LEFT))
		{
			ret |= Direction::RIGHT;
		}
		else if (static_cast<unsigned int>(D & Direction::RIGHT))
		{
			ret |= Direction::LEFT;
		}

		if (static_cast<unsigned int>(D & Direction::UP))
		{
			ret |= Direction::DOWN;
		}
		else if (static_cast<unsigned int>(D & Direction::DOWN))
		{
			ret |= Direction::UP;
		}
		return ret;
	}

	bool MapCharacter::g_stopped = false;

	MapCharacter::MapCharacter(TileMap* map, unsigned int r, unsigned int c, float s, bool friendly)
		:m_map(map), m_speed(s), m_cWidth(16), m_cHeight(16), m_sprite(sf::seconds(1 / 12.f), true, true), m_angle(convertToAngle(Direction::DOWN)),
		m_hitBox(sf::RectangleShape(sf::Vector2f(16, 16))), m_friendly(friendly)
	{
		setPosition((c + 0.5f) * m_map->getTileSize(), (r + 0.5f) * m_map->getTileSize());
		m_lastPosition = m_currPosition = getPosition();
		updateHitbox();
	}

	MapCharacter::MapCharacter(float speed, bool side)
		:m_speed(speed), m_friendly(side), m_sprite(sf::seconds(1 / 12.f), true, true), m_angle(convertToAngle(Direction::DOWN)), 
		m_hitBox(sf::RectangleShape(sf::Vector2f(16, 16)), true), m_cWidth(16), m_cHeight(16)
	{
	}

	MapCharacter::~MapCharacter()
	{}

	bool MapCharacter::loadFromFile(const std::string & filename)
	{
		std::ifstream in;
		in.open(filename, std::ios::in);

		if (!in.is_open())
		{
			return false;
		}

		std::string texture;
		in >> texture;

		unsigned int size, w, h;
		in >> size >> w >> h;
		setCollision(w, h);

		const Direction dir[] = { Direction::DOWN, Direction::LEFT, Direction::RIGHT, Direction::UP,
			Direction::DOWN | Direction::LEFT, Direction::LEFT | Direction::UP, Direction::DOWN | Direction::RIGHT, Direction::UP | Direction::RIGHT};

		for (int i = 0; i <	sizeof(dir) / sizeof(Direction); i++)
		{
			std::shared_ptr<Animation> anim = std::make_shared<Animation>(texture);

			for (int j = 0; j < 4; j++)
			{
				unsigned int x, y;
				in >> x >> y;
				anim->addFrame(sf::IntRect(x, y, size, size));
			}
			addAnimation(dir[i], anim);
		}
		return true;
	}

	void MapCharacter::addAnimation(Direction d, std::shared_ptr<Animation> anim)
	{
		m_animations[d] = anim;
		if (!m_sprite.getAnimation())
		{
			m_sprite.setAnimation(m_animations[d]);
			auto dim = m_sprite.getLocalBounds();
			m_sprite.setOrigin(dim.width / 2, dim.height - m_cHeight / 2);			
		}
	}

	void MapCharacter::setCollision(float width, float height)
	{
		m_cWidth = width;
		m_cHeight = height;
		m_hitBox.setSize(sf::Vector2f(m_cWidth, m_cHeight));
		m_hitBox.setOrigin(width / 2.f, height / 2.f);
		if (m_sprite.getAnimation())
		{
			auto dim = m_sprite.getLocalBounds();
			m_sprite.setOrigin(dim.width / 2, dim.height - m_cHeight / 2);
		}
	}

	void MapCharacter::setColor(const sf::Color& c)
	{
		m_sprite.setColor(c);
	}

	void MapCharacter::setTile(const sf::Vector2u & dest)
	{
		setPosition((dest.x + 0.5f) * m_map->getTileSize(), (dest.y + 0.5f) * m_map->getTileSize());
		m_lastPosition = m_currPosition = getPosition();
		updateHitbox();
	}

	void MapCharacter::move(float magnitude, float angle)
	{
		sf::Vector2f change(magnitude * std::cos(angle), magnitude * std::sin(angle));
		auto nextPos = m_currPosition + change;
		calculateCorners(sf::Vector2f(m_currPosition.x, nextPos.y));

		auto currTile = getTile(m_currPosition);

		if (change.y < 0.0f)
		{
			if (!m_topLeft || !m_topRight)
			{
				nextPos.y = currTile.y * m_map->getTileSize() + m_cHeight / 2;
			}
		}
		else if (change.y > 0.0f)
		{
			if (!m_botLeft || !m_botRight)
			{
				nextPos.y = (currTile.y + 1) * m_map->getTileSize() - m_cHeight / 2;
			}
		}

		calculateCorners(sf::Vector2f(nextPos.x, m_currPosition.y));

		if (change.x < 0.0f)
		{
			if (!m_topLeft || !m_botLeft)
			{
				nextPos.x = currTile.x * m_map->getTileSize() + m_cWidth / 2;
			}
		}
		else if (change.x > 0.0f)
		{
			if (!m_topRight || !m_botRight)
			{
				nextPos.x = (currTile.x + 1) * m_map->getTileSize() - m_cWidth / 2;
			}
		}

		m_currPosition = nextPos;
		updateHitbox();

	}

	const sf::Vector2f & MapCharacter::getLastPosition() const
	{
		return m_lastPosition;
	}

	bool MapCharacter::contact(const MapCharacter & p)
	{
		if (g_stopped)
		{
			return false;
		}
		return m_hitBox.getGlobalBounds().contains(p.getCurrPosition());
	}

	const THitBox<sf::RectangleShape>& MapCharacter::getHitBox() const
	{
		return m_hitBox;
	}

	const sf::Vector2f& MapCharacter::getCurrPosition() const
	{
		return m_currPosition;
	}

	bool MapCharacter::getFriendly() const
	{
		return m_friendly;
	}

	float MapCharacter::getAngle() const
	{
		return m_angle;
	}

	float MapCharacter::getCollisionWidth() const
	{
		return m_cWidth;
	}

	float MapCharacter::getCollisionHeight() const
	{
		return m_cHeight;
	}

	sf::Vector2f MapCharacter::getCenter() const
	{
		sf::Vector2f res(getPosition() + sf::Vector2f(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f));
		return sf::Vector2f(std::roundf(res.x), std::roundf(res.y));
	}

	TileMap * MapCharacter::getTileMap()
	{
		return m_map;
	}

	void MapCharacter::setIntention(Direction d)
	{
		m_intention = d;
	}

	void MapCharacter::setStopped(bool b)
	{
		g_stopped = b;
	}

	bool MapCharacter::getStopped()
	{
		return g_stopped;
	}

	void MapCharacter::fixedUpdate()
	{
		if (g_stopped)
		{
			m_intention = Direction::NONE;
		}
		updateLogic();
		updatePos();
	}

	void MapCharacter::update()
	{
		float interpol = wv::IApp::instance()->getTime().getDeltaLogicTime() /
			wv::IApp::instance()->getTime().getTicksPerFrame();

		setPosition(m_lastPosition * (1.0f - interpol) + m_currPosition * interpol);
		m_sprite.update(wv::IApp::instance()->getTime().getDeltaRenderTime());
	}

	const sf::Drawable & MapCharacter::getDrawable(const sf::View &)
	{
		return m_sprite;
	}

	bool MapCharacter::operator<(const MapCharacter & m)
	{
		return m_currPosition.x < m.m_currPosition.x;
	}

	void MapCharacter::calculateCorners(sf::Vector2f pos)
	{
		auto mapPos = getTile(pos + sf::Vector2f(-m_cWidth / 2, -m_cHeight / 2));
		m_topLeft = m_map->canMove(mapPos.y, mapPos.x);
		mapPos = getTile(pos + sf::Vector2f(m_cWidth / 2 - 1, -m_cHeight / 2));
		m_topRight = m_map->canMove(mapPos.y, mapPos.x);
		mapPos = getTile(pos + sf::Vector2f(-m_cWidth / 2, m_cHeight / 2 - 1));
		m_botLeft = m_map->canMove(mapPos.y, mapPos.x);
		mapPos = getTile(pos + sf::Vector2f(m_cWidth / 2 - 1, m_cHeight / 2 - 1));
		m_botRight = m_map->canMove(mapPos.y, mapPos.x);
	}

	void MapCharacter::updatePos()
	{
		m_lastPosition = m_currPosition;
		auto nextPos = m_currPosition + m_velocity;
		calculateCorners(sf::Vector2f(m_currPosition.x, nextPos.y));

		auto currTile = getTile(m_currPosition);

		if (m_velocity.y < 0.0f)
		{
			if (!m_topLeft || !m_topRight)
			{
				nextPos.y = currTile.y * m_map->getTileSize() + m_cHeight / 2;
			}
		}
		else if (m_velocity.y > 0.0f)
		{
			if (!m_botLeft || !m_botRight)
			{
				nextPos.y = (currTile.y + 1) * m_map->getTileSize() - m_cHeight / 2;
			}
		}

		calculateCorners(sf::Vector2f(nextPos.x, m_currPosition.y));

		if (m_velocity.x < 0.0f)
		{
			if (!m_topLeft || !m_botLeft)
			{
				nextPos.x = currTile.x * m_map->getTileSize() + m_cWidth / 2;
			}
		}
		else if (m_velocity.x > 0.0f)
		{
			if (!m_topRight || !m_botRight)
			{
				nextPos.x = (currTile.x + 1) * m_map->getTileSize() - m_cWidth / 2;
			}
		}

		m_currPosition = nextPos;
		updateHitbox();
	}

	void MapCharacter::updateLogic()
	{
		if (m_isMoving)
		{
			if (m_intention == Direction::NONE)
			{
				stopMoving();
			}
			else if (m_intention != m_lastMove)
			{
				changeDirection(m_intention);
			}
		}
		else if (m_intention != Direction::NONE)
		{
			startMoving(m_intention);
		}

		m_lastMove = m_intention;
	}

	void MapCharacter::updateHitbox()
	{
		m_hitBox.setPosition(m_currPosition);
	}

	void MapCharacter::stopMoving()
	{
		m_isMoving = false;
		m_velocity *= 0.0f;
		m_sprite.stop();
	}

	void MapCharacter::startMoving(Direction d)
	{
		if (static_cast<int>(d & Direction::UP))
		{
			if (d == Direction::UP)
			{
				m_velocity.y = -m_speed;
			}
			else
			{
				m_velocity.y = -m_speed * std::sin(3.14159265 / 4);
			}
		}
		else if (static_cast<int>(d & Direction::DOWN))
		{
			if (d == Direction::DOWN)
			{
				m_velocity.y = m_speed;
			}
			else
			{
				m_velocity.y = m_speed * std::sin(3.14159265 / 4);
			}
		}

		if (static_cast<int>(d & Direction::LEFT))
		{
			if (d == Direction::LEFT)
			{
				m_velocity.x = -m_speed;
			}
			else
			{
				m_velocity.x = -m_speed * std::cos(3.14159265 / 4);
			}
		}
		else if (static_cast<int>(d & Direction::RIGHT))
		{
			if (d == Direction::RIGHT)
			{
				m_velocity.x = m_speed;
			}
			else
			{
				m_velocity.x = m_speed * std::cos(3.14159265 / 4);
			}
		}

		m_isMoving = true;
		m_lastMove = d;
		m_angle = convertToAngle(d);

		m_sprite.setAnimation(m_animations[d]);
		m_sprite.play();
		m_sprite.setLooped(true);
	}

	void MapCharacter::changeDirection(Direction d)
	{
		stopMoving();
		startMoving(d);
	}

	sf::Vector2i MapCharacter::getTile(sf::Vector2f p)
	{
		return sf::Vector2i(static_cast<int>(std::floorf((p.x / m_map->getTileSize()))),
			static_cast<int>((std::floorf(p.y / m_map->getTileSize()))));
	}

}