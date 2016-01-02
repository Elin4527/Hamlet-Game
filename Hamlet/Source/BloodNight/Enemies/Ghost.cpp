#include "Ghost.hpp"

Ghost::Ghost(wv::TileMap* t, unsigned int r, unsigned int c)
	:wv::Enemy("Ghost", t, r, c, 3.0f, 1)
{
	loadFromFile("Data/Characters/sr.txt");
	setColor(sf::Color(255, 255, 255, 155));
	setNonAttackable(true);
}

Ghost::Ghost()
	:wv::Enemy("Ghost", 3.0f, 1)
{
	loadFromFile("Data/Characters/sr.txt");
	setColor(sf::Color(255, 255, 255, 155));
	setNonAttackable(true);
}

Ghost::~Ghost()
{
}

void Ghost::fixedUpdate()
{
	auto hamlet = getTileMap()->getPlayer();
	setIntention(wv::getOpposite(track(*hamlet)));
	Enemy::fixedUpdate();
}

void Ghost::collided(wv::Player & p)
{
	damage(1);
}

std::shared_ptr<wv::Enemy> Ghost::spawn(wv::TileMap * t, unsigned int r, unsigned int c)
{
	return std::make_shared<Ghost>(t, r, c);
}
