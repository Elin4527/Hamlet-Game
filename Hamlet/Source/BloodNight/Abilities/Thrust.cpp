#include "Thrust.hpp"

std::shared_ptr<wv::Animation> Thrust::g_anim = nullptr;
unsigned int Thrust::g_ref = 0;

Thrust::Thrust(sf::Time cd)
	:wv::IAbility("Thrust", "Images/Icons/thrust.png", cd)
{
	g_ref++;
	if (!g_anim)
	{
		g_anim = std::make_shared<wv::Animation>("Images/Animations/thrustAnim.png");
		g_anim->addFrame(sf::IntRect(0, 0, 20, 10));
		g_anim->addFrame(sf::IntRect(20, 0, 20, 10));
		g_anim->addFrame(sf::IntRect(40, 0, 20, 10));
	}
}

Thrust::~Thrust()
{
	g_ref--;
	if (!g_ref)
	{
		g_anim = nullptr;
	}
}

void Thrust::use(wv::Battler & p)
{
	IAbility::use(p);
	float angle = p.getAngle();
	float radius = 10.f;
	float distance = 5.f;
	wv::THitBox<sf::RectangleShape> hitbox(sf::RectangleShape(sf::Vector2f(20, 10)), false);
	hitbox.rotate(angle * 180 / 3.14159265);
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

wv::IAbility * Thrust::clone() const
{
	return new Thrust(getCoolDown());
}

void Thrust::effect(wv::Battler & user, wv::Battler& target)
{
	if (target.getFriendly())
	{
		target.damage(1);
	}
	else if (target.isAttackable())
	{
		target.damage(15);
	}
	target.move(16.f, user.getAngle());
}
