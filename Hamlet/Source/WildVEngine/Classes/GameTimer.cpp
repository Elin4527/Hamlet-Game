#include "GameTimer.hpp"

namespace wv
{

	GameTimer::GameTimer(unsigned int fps)
		:m_updateRate(sf::microseconds(1000000 / fps)), 
		m_nextFrame(sf::microseconds(1000000 / fps))
	{
	}

	void GameTimer::setFPS(unsigned int fps)
	{
		m_updateRate = sf::microseconds(1000000 / fps);
		m_nextFrame = m_lastFrame + m_updateRate;
	}

	void GameTimer::reset()
	{
		m_nextFrame = m_updateRate;
		m_prevTime = m_currentTime = m_lastFrame = sf::Time::Zero;
		m_clock.restart();
	}

	void GameTimer::update()
	{
		m_prevTime = m_currentTime;
		m_currentTime = m_clock.getElapsedTime();
	}

	// Call only once per frame
	bool GameTimer::nextFrame()
	{
		// Automatically updates the timer when return true
		if (m_currentTime > m_nextFrame)
		{
			m_lastFrame = m_nextFrame;
			m_nextFrame += m_updateRate;
			return true;
		}
		return false;
	}

	const sf::Time& GameTimer::getTicksPerFrame() const
	{
		return m_updateRate;
	}

	sf::Time GameTimer::getDeltaRenderTime() const
	{
		return m_currentTime - m_prevTime;
	}

	sf::Time GameTimer::getDeltaLogicTime() const
	{
		return m_currentTime - m_lastFrame;
	}

	const sf::Time& GameTimer::getTime() const
	{
		return m_currentTime;
	}

}