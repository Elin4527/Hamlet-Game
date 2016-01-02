#pragma once
#include <string>
#include <SFML\Graphics.hpp>
#include "../Interfaces/TResource.hpp"

namespace wv
{
	class Player;

	// Items stored in the players inventory to be used
	class IItem
	{
		std::string m_name;
		TResource<sf::Texture> m_texture;
	public:
		virtual ~IItem();
		virtual void use(Player& p) = 0;
		sf::Texture& getTexture();
		const std::string& getName() const;
	protected:
		IItem(const std::string& itemName, const std::string& texture);
	};


}