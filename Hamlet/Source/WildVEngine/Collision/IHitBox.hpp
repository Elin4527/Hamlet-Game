#pragma once
#include <SFML\Graphics.hpp>

namespace wv
{
	// An interface for HitBox that allows for collision detection
	// using seperate axis theorem

	class IHitBox
	{
		bool m_visible;
	public:
		template<class T>friend class THitBox;
		IHitBox(bool vis);
		virtual ~IHitBox();
		virtual bool collision(const IHitBox& rhs) const = 0;

		void setVisibility(bool vis);
		bool isVisible() const;
	protected:
		virtual bool sat(const IHitBox& rhs) const = 0;
		virtual sf::Vector2f project(const sf::Vector2f& axis) const = 0;
	};

}