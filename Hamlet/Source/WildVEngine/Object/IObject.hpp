#pragma once
#include <SFML/Graphics.hpp>

namespace wv
{

	class IObject: public sf::Transformable
	{
	public:
		virtual ~IObject() = 0;	
		virtual void setColor(const sf::Color& c) = 0;
	};

	class RenderComponent: public virtual IObject
	{
	public:
		virtual ~RenderComponent() {}
		virtual void update() {}
		virtual void lateUpdate() {}
		virtual const sf::Drawable& getDrawable(const sf::View&) = 0;
	};

	class LogicComponent: public virtual IObject
	{
	public:
		virtual ~LogicComponent() {}
		virtual bool handleInput(sf::Event& e) { return false; }
		virtual void fixedUpdate() = 0;
	};
}