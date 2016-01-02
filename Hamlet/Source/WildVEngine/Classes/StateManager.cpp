#include "StateManager.hpp"
#include <iostream>

namespace wv
{

	StateManager::~StateManager()
	{
		while (!m_activeStates.empty())
		{
			pop();
		}
	}

	void StateManager::push(const std::shared_ptr<IState>& state, unsigned int mode)
	{
		m_activeStates.push_back(std::make_pair(state, mode));

		if (mode == modality::EXCLUSIVE)
		{
			m_exposedRenderables.clear();
			m_exposedUpdateables.clear();
		}
		addToExposed(state.get());		

		if (mode == modality::EXCLUSIVE)
			notifyObscuredStates();

		state->entered();
	}

	std::shared_ptr<IState> StateManager::peek() const
	{
		if (m_activeStates.empty())
		{
			return std::shared_ptr<IState>();
		}
		return m_activeStates.at(m_activeStates.size() - 1).first;
	}

	std::shared_ptr<IState> StateManager::pop()
	{
		if (m_activeStates.empty())
		{
			throw std::runtime_error("Attempted to pop from an empty game stack");
		}

		auto popped = m_activeStates.at(m_activeStates.size() - 1);
		popped.first->exiting();
		m_activeStates.pop_back();

		// Remove from queues and rebuild if necessary
		removeFromExposed(popped.first.get());
		if (popped.second == modality::EXCLUSIVE)
		{
			rebuildQueue(m_exposedRenderables, modality::EXCLUSIVE);
			rebuildQueue(m_exposedUpdateables, modality::EXCLUSIVE);
		}
		notifyRevealedStates();
		
		return popped.first;
	}

	std::shared_ptr<IState> StateManager::change(const std::shared_ptr<IState>& state, unsigned int mode)
	{
		auto currentState = peek();
		if (currentState)
		{
			pop();
		}
		push(state, mode);
		return currentState;
	}

	bool StateManager::processInput(sf::Event e)
	{
		if (m_exposedUpdateables.empty())
		{
			return false;
		}
		return m_exposedUpdateables.back()->handleInput(e);
	}

	void StateManager::logicUpdate()
	{
		for (auto& s : m_exposedUpdateables)
		{
			s->fixedUpdate();
		}
	}
	
	void StateManager::renderUpdate()
	{
		for (auto& s : m_exposedRenderables)
		{
			s->update();
		}
		for (auto& s : m_exposedRenderables)
		{
			s->lateUpdate();
		}
	}

	void StateManager::render(sf::RenderTarget& rt)
	{

		for (auto& s : m_exposedRenderables)
		{
			s->render(rt);
		}
	}

	void StateManager::addToExposed(IState* state)
	{
		addToQueue(state, m_exposedRenderables);
		addToQueue(state, m_exposedUpdateables);
	}

	void StateManager::removeFromExposed(IState* state)
	{
		Renderable* renderable = dynamic_cast<Renderable*>(state);
		if (renderable)
		{
			m_exposedRenderables.pop_back();
		}

		Updateable* updateable = dynamic_cast<Updateable*>(state);
		if (updateable)
		{
			m_exposedUpdateables.pop_back();
		}
	}

	template<class T>
	void StateManager::addToQueue(IState * state, std::vector<T*>& queue)
	{
		T* ptr = dynamic_cast<T*>(state);
		if (ptr)
		{
			queue.push_back(ptr);
		}
	}

	template <class T>
	void StateManager::rebuildQueue(std::vector<T*>& queue, unsigned int flag)
	{
		queue.clear();
		if (m_activeStates.empty()) {
			return;
		}

		// Reverse scan the active states until the beginning or mode == flag
		auto index = m_activeStates.size() - 1;
		while (index > 0)
		{
			if (m_activeStates.at(index).second == flag) {
				break;
			}
			--index;
		}

		// Go forward and fill the queue
		for (; index < m_activeStates.size(); index++)
		{
			T* ptr = dynamic_cast<T*>(m_activeStates.at(index).first.get());
			if (ptr)
			{
				queue.push_back(ptr);
			}
		}
	}

	void StateManager::notifyObscuredStates()
	{
		if (m_activeStates.size() < 2)
			return;

		// Reverse scan until we hit either the beginning or find the next Hiding state
		std::size_t index = m_activeStates.size() - 2;
		while (index > 0)
		{
			if (m_activeStates.at(index).second == modality::EXCLUSIVE) {
				break;
			}
			--index;
		}

		// Now go forward (up until the second-to-last state) and notify the obscured states
		while (index < m_activeStates.size() - 1)
		{
			m_activeStates.at(index).first->obscuring();
			index++;

		}
	}

	void StateManager::notifyRevealedStates()
	{
		if (m_activeStates.empty())
			return;

		// Reverse scan until we hit either the beginning or find the next Hiding state
		std::size_t index = m_activeStates.size() - 1;
		while (index > 0)
		{
			if (m_activeStates.at(index).second == modality::EXCLUSIVE) {
				break;
			}
			--index;
		}

		// Now go forward and notify all revealed state
		while (index < m_activeStates.size())
		{
			m_activeStates.at(index).first->revealed();
			index++;
		}
	}

	
}