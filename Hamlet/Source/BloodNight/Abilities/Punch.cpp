#include "Punch.hpp"

std::shared_ptr<wv::Animation> Punch::g_anim = nullptr;
unsigned int Punch::g_ref = 0;

Punch::Punch(sf::Time cd)
	:wv::IAbility("Punch", "Images/Icons/punch.png", cd)
{
	g_ref++;
	if (!g_anim)
	{
		g_anim = std::make_shared<wv::Animation>("Images/Animations/punchAnim.png");
		g_anim->addFrame(sf::IntRect(0, 0, 20, 20));
		g_anim->addFrame(sf::IntRect(20, 0, 20, 20));
		g_anim->addFrame(sf::IntRect(40, 0, 20, 20));
	}
}

Punch::~Punch()
{
	g_ref--;
	if (!g_ref)
	{
		g_anim = nullptr;
	}
}

void Punch::use(wv::Battler & p)
{
	IAbility::use(p);
	float angle = p.getAngle();
	float radius = 10.f;
	float distance = 5.f;
	wv::THitBox<sf::CircleShape> hitbox(sf::CircleShape(radius), false);
	sf::Vector2f center = p.getCurrPosition() + sf::Vector2f((radius + distance + p.getCollisionWidth() / 2.0f) * std::cos(angle),
		(radius + distance + p.getCollisionHeight() / 2.0f) * std::sin(angle));
	hitbox.setPosition(center);

	wv::AnimatedSprite s;
	s.setLooped(false);
	s.setAnimation(g_anim);
	s.setOrigin(s.getLocalBounds().width / 2.0f, s.getLocalBounds().height / 2.0f);
	s.setRotation(angle * 180 / 3.14159265);
	s.setPosition(center);

	p.getTileMap()->addVisualEffect(s);

	if (p.getFriendly())
	{
		auto list = p.getTileMap()->getEnemies(center.x - radius, center.x + radius);
		for (auto ptr: list)
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

wv::IAbility * Punch::clone() const
{
	return new Punch(getCoolDown());
}

void Punch::effect(wv::Battler & user, wv::Battler& target)
{
	if (target.getFriendly())
	{
		target.damage(1);
	}
	else if(target.isAttackable())
	{
		target.damage(5);
	}
	target.move(8.f, user.getAngle());
}
