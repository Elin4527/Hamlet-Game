#include "Animation.hpp"

namespace wv
{
	Animation::Animation()
	{}

	Animation::Animation(const std::string & image)
		:m_spriteSheet(image)
	{
	}

	void Animation::addFrame(const sf::IntRect& f)
	{
		m_frames.push_back(f);
	}

	void Animation::setSpriteSheet(const std::string& s)
	{
		m_spriteSheet.setID(s, LOAD_LATER);
	}

	const sf::IntRect& Animation::getFrame(std::size_t i) const
	{
		return m_frames[i];
	}

	const sf::Texture& Animation::getSpriteSheet()
	{
		return m_spriteSheet.getResource();
	}

	std::size_t Animation::size() const
	{
		return m_frames.size();
	}
}