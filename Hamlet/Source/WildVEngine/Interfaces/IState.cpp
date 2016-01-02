#include "IState.hpp"

namespace wv
{
	// Makes an abstract class
	IState::~IState() {}

	void IState::finish()
	{
		m_done = true;
	}

	void IState::setChange(std::shared_ptr<IState> next, unsigned int mode)
	{
		m_nextState = next;
		m_nextMode = mode;
		m_change = true;
	}

	void IState::setAdd(std::shared_ptr<IState> next, unsigned int mode)
	{
		m_nextState = next;
		m_nextMode = mode;

		m_add = true;
	}

	bool IState::isDone()
	{
		return m_done;
	}
	bool IState::isAdding()
	{
		return m_add;
	}
	bool IState::isChanging()
	{
		return m_change;
	}
	std::shared_ptr<IState> IState::getNextState()
	{
		m_add = false;
		m_change = false;
		return m_nextState;
	}
	unsigned int IState::getNextMode()
	{
		return m_nextMode;
	}
}