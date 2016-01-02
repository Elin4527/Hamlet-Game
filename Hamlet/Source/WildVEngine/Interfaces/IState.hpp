#pragma once

#include <SFML\Graphics.hpp>
#include <memory>

namespace wv
{
	// Based on the game state manager found here: http://blog.nuclex-games.com/tutorials/cxx/game-state-management/

	namespace modality
	{
		enum Modes {
			EXCLUSIVE,
			POPUP
		};
	}

	class IState
	{
		std::shared_ptr<IState> m_nextState;
		unsigned int m_nextMode;
		bool m_add;
		bool m_change;
		bool m_done;
	public:

		virtual ~IState() = 0;
		virtual void entered() {}
		virtual void exiting() {}
		virtual void obscuring() {}
		virtual void revealed() {}

		bool isDone();
		bool isAdding();
		bool isChanging();

		std::shared_ptr<IState> getNextState();
		unsigned int getNextMode();
	protected:
		void finish();
		void setChange(std::shared_ptr<IState> next, unsigned int mode);
		void setAdd(std::shared_ptr<IState> next, unsigned int mode);
		IState() : m_done(false), m_change(false), m_add(false) {}
	};

	class Updateable
	{
	public:
		virtual ~Updateable() {}
		virtual bool handleInput(sf::Event& e)
		{
			return false;
		}
		virtual void fixedUpdate() {};
	};

	class Renderable
	{
	public:
		virtual ~Renderable() {}
		virtual void update() {}
		virtual void lateUpdate() {}
		virtual void render(sf::RenderTarget& rt) = 0;
	};
}