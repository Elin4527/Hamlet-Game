#include "Flurry.hpp"

std::shared_ptr<wv::Animation> Flurry::g_anim = nullptr;
unsigned int Flurry::g_ref = 0;

Flurry::Flurry(sf::Time cd)
	:wv::IAbility("Flurry", "Images/Icons/flurry.png", cd)
{
	g_ref++;
	if (!g_anim)
	{
		g_anim = std::make_shared<wv::Animation>("Images/Animations/flurryAnim.png");
		g_anim->addFrame(sf::IntRect(0, 0, 30, 60));
		g_anim->addFrame(sf::IntRect(30, 0, 30, 60));
		g_anim->addFrame(sf::IntRect(60, 0, 30, 60));
	}
}

Flurry::~Flurry()
{
	g_ref--;
	if (!g_ref)
	{
		g_anim = nullptr;
	}
}

void Flurry::use(wv::Battler & p)
{
	IAbility::use(p);
	float angle = p.getAngle();
	float radius = 15.f;
	float distance = 5.f;
	wv::THitBox<sf::RectangleShape> hitbox(sf::RectangleShape(sf::Vector2f(30, 60)), false);
	hitbox.rotate(angle * 180 / 3.14159265);
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

wv::IAbility * Flurry::clone() const
{
	return new Flurry(getCoolDown());
}

void Flurry::effect(wv::Battler & user, wv::Battler& target)
{
	if (target.getFriendly())
	{
		target.damage(1);
	}
	else if (target.isAttackable())
	{
		target.damage(40);
	}
	target.move(20.f, user.getAngle() + wv::convertToAngle(wv::Direction::DOWN | wv::Direction::RIGHT));
}
