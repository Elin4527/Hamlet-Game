#include "IItem.hpp"

namespace wv
{

	IItem::IItem(const std::string& name, const std::string& texture)
		:m_name(name), m_texture(texture)
	{}

	IItem::~IItem()
	{}

	sf::Texture & IItem::getTexture()
	{
		return m_texture.getResource();
	}

	const std::string & IItem::getName() const
	{
		return m_name;
	}

}