#pragma once

#include "..\Interfaces\IState.hpp"
#include <memory>
#include <vector>

namespace wv
{
	// Used to manage state of the game
	// Based on the game state manager found here: http://blog.nuclex-games.com/tutorials/cxx/game-state-management/

	class StateManager
	{
		typedef std::pair<std::shared_ptr<IState>, unsigned int> StateModePair;

		std::vector<StateModePair> m_activeStates;			// All states on the stack
		std::vector<Renderable*> m_exposedRenderables;		// All currently rendering states
		std::vector<Updateable*> m_exposedUpdateables;		// All currently logically updating states
	public:
		~StateManager();
		
		void push(const std::shared_ptr<IState>& state, unsigned int mode = modality::EXCLUSIVE);
		std::shared_ptr<IState> peek() const;
		std::shared_ptr<IState> pop();
		std::shared_ptr<IState> change(const std::shared_ptr<IState>& state, unsigned int mode = modality::EXCLUSIVE);
	
		bool processInput(sf::Event e);		// Returns whether an event has been processed
		void logicUpdate();
		void renderUpdate();
		void render(sf::RenderTarget& rt);
	private:

		// Helper Functions
		void addToExposed(IState* state);
		template<class T>
		void addToQueue(IState* state, std::vector<T*>& queue);
		void removeFromExposed(IState* state);

		template<class T>
		void rebuildQueue(std::vector<T*>& queue, unsigned int flag);

		void notifyObscuredStates();
		void notifyRevealedStates();
	};

}