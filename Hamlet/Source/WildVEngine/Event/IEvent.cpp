#include "IEvent.hpp"

namespace wv
{
	IEvent::IEvent()
		:m_active(true)
	{}

	void IEvent::setColor(const sf::Color & c)
	{
	}

	bool IEvent::isActive() const
	{
		return m_active;
	}

	void IEvent::done()
	{
		m_active = false;
	}

	IEvent::~IEvent()
	{}

	void IEvent::collided(Player& p)
	{}

	void IEvent::interact(Player& p)
	{}

}