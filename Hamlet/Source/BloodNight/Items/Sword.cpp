#include "Sword.hpp"
#include "../Abilities/Thrust.hpp"
#include "../Abilities/LSlash.hpp"
#include "../Abilities/RSlash.hpp"
#include "../Abilities/Flurry.hpp"

Sword::Sword()
	:wv::EquipItem("Sword", "Images/Icons/sword.png")
{
	addAbility(&RSlash(sf::seconds(0.6f)));
	addAbility(&LSlash(sf::seconds(0.6f)));
	addAbility(&Thrust(sf::seconds(0.8f)));
	addAbility(&Flurry(sf::seconds(5.0f)));
}

Sword::~Sword()
{
}