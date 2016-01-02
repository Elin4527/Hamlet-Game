#pragma once

#include "../Interfaces/TResource.hpp"
#include "../Object/Battler.hpp"
#include "../Object/TileMap.hpp"

namespace wv
{
	// Class that defines actions usable by characters in the game
	class IAbility
	{
		TResource<sf::Texture> m_icon;	// Icon for Player GUI
		sf::Time m_coolDown;
		sf::Time m_remainingCD;
		const std::string m_name;
	public:
		virtual ~IAbility();

		virtual void use(Battler& p);
		void update(const sf::Time& t);
		virtual IAbility* clone() const = 0;

		// Getters
		sf::Texture& getTexture();
		const sf::Time& getRemainingTime() const;
		const sf::Time& getCoolDown() const;
		const std::string& getName() const;
	
	protected:
		// Protected constructor to prevent an IAbility from being instantiated as it is abstract
		IAbility(const std::string& name, const std::string& texture, const sf::Time& cooldown);

		// Define what happens when a unit is hit by another units ability
		virtual void effect(Battler& user, Battler& target);
	};

}
