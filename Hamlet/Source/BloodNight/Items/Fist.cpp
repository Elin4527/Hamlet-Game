#include "Fist.hpp"
#include "../Abilities/Punch.hpp"

Fist::Fist()
	:wv::EquipItem("Fist", "Images/Icons/fist.png")
{
	addAbility(&Punch(sf::seconds(1.0f)));
	addAbility(nullptr);
	addAbility(nullptr);
	addAbility(nullptr);
}

Fist::~Fist()
{
}

