#pragma once

#include <SFML\System\Clock.hpp>

namespace wv
{
	// Class used to track the time and frame deltas for the overall game
	class GameTimer
	{
	public:
		GameTimer(unsigned int fps);
		void setFPS(unsigned int fps);
		void update();
		void reset();
		bool nextFrame();

		const sf::Time& getTicksPerFrame() const;

		// Returns the time since last rendered frame
		sf::Time getDeltaRenderTime() const;

		// Returns the time since last logic update
		sf::Time getDeltaLogicTime() const;

		// Returns time since start of program
		const sf::Time& getTime() const;

	private:
		sf::Clock m_clock;
		sf::Time m_currentTime;
		sf::Time m_prevTime;
		sf::Time m_frameDelta;
		sf::Time m_nextFrame;
		sf::Time m_lastFrame;
		sf::Time m_updateRate;
	};
}