#include "LSlash.hpp"

std::shared_ptr<wv::Animation> LSlash::g_anim = nullptr;
unsigned int LSlash::g_ref = 0;

LSlash::LSlash(sf::Time cd)
	:wv::IAbility("LSlash", "Images/Icons/lslash.png", cd)
{
	g_ref++;
	if (!g_anim)
	{
		g_anim = std::make_shared<wv::Animation>("Images/Animations/lslashAnim.png");
		g_anim->addFrame(sf::IntRect(0, 0, 15, 30));
		g_anim->addFrame(sf::IntRect(15, 0, 15, 30));
		g_anim->addFrame(sf::IntRect(30, 0, 15, 30));
	}
}

LSlash::~LSlash()
{
	g_ref--;
	if (!g_ref)
	{
		g_anim = nullptr;
	}
}

void LSlash::use(wv::Battler & p)
{
	IAbility::use(p);
	float angle = p.getAngle();
	float radius = 7.5f;
	float distance = 5.f;
	wv::THitBox<sf::RectangleShape> hitbox(sf::RectangleShape(sf::Vector2f(15, 30)), false);
	hitbox.setRotation(angle * 180 / 3.14159265);
	sf::Vector2f center = p.getCurrPosition() + sf::Vector2f((radius + distance + p.getCollisionWidth() / 2.0f) * std::cos(angle),
		(radius + distance + p.getCollisionHeight() / 2.0f) * std::sin(angle));
	hitbox.setPosition(center);
	wv::AnimatedSprite s(sf::seconds(1.f / 12.f), false, false);
	s.setLooped(false);
	s.setAnimation(g_anim);
	s.setOrigin(s.getLocalBounds().width / 2.0f, s.getLocalBounds().height / 2.0f);
	s.setRotation(angle * 180 / 3.14159265);
	s.setPosition(center);

	p.getTileMap()->addVisualEffect(s);

	if (p.getFriendly())
	{
		auto list = p.getTileMap()->getEnemies(hitbox.getGlobalBounds().left, hitbox.getGlobalBounds().left + hitbox.getGlobalBounds().width);
		for (auto ptr : list)
		{
			if (ptr)
			{
				if (hitbox.collision(ptr->getHitBox()))
				{
					effect(p, *ptr);
				}
			}
		}
	}
	else
	{
		auto ptr = p.getTileMap()->getPlayer();
		if (hitbox.collision(ptr->getHitBox()))
		{
			effect(p, *ptr);
		}
	}
}

wv::IAbility * LSlash::clone() const
{
	return new LSlash(getCoolDown());
}

void LSlash::effect(wv::Battler & user, wv::Battler& target)
{
	if (target.getFriendly())
	{
		target.damage(1);
	}
	else if (target.isAttackable())
	{
		target.damage(10);
	}
	target.move(16.f, user.getAngle() + wv::convertToAngle(wv::Direction::UP | wv::Direction::RIGHT));
}
