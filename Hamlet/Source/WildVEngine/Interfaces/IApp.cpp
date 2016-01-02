#include "IApp.hpp"
#include <iostream>

namespace wv
{
	IApp* IApp::g_pApp = nullptr;

	IApp::IApp(const std::string& title, unsigned int width, unsigned int height, float scale, unsigned long style)
		: m_title(title), m_scale(scale), m_windowStyle(style), m_videoMode(width*scale, height*scale),
		m_window(), m_maxUpdates(5), m_time(20), m_width(width), m_height(height),
		m_exitCode(0), m_running(false)
	{
		g_pApp = this;

	}

	void IApp::gameLoop()
	{
		m_time.reset();
		sf::Uint32 loop;
		while (m_running && m_window.isOpen())
		{
			loop = 0;
			m_time.update();
			while (loop < m_maxUpdates && m_time.nextFrame())
			{
				loop++;
				processInput();
				update();
				auto state = m_stateManager.peek();
				if (state->isDone())
				{
					m_stateManager.pop();
				}
				else if (state->isAdding())
				{
					m_stateManager.push(state->getNextState(), state->getNextMode());
				}
				else if (state->isChanging())
				{
					m_stateManager.change(state->getNextState(), state->getNextMode());
				}
			}

			render();
		}
	}

	void IApp::processInput()
	{
		sf::Event e;
		while (m_window.pollEvent(e))
		{
			if (m_stateManager.processInput(e))
				continue;

			// Default Event handles if StateManager has been unable to handle
			switch (e.type)
			{
				case sf::Event::Closed:
					m_window.close();
					break;
				case sf::Event::Resized:
				{
					float xRat = static_cast<float>(e.size.width) / static_cast<float>(getWidth());
					float yRat = static_cast<float>(e.size.height) / static_cast<float>(getHeight());

					// Scale in the direction with greatest absolute change
					if (std::abs(xRat - m_scale) > std::abs(yRat - m_scale))
					{
						m_scale = xRat;
					}
					else
					{
						m_scale = yRat;
					}

					// Resize the window with aspect ratio and scale the camera
					m_window.setSize(sf::Vector2u(static_cast<unsigned int>(static_cast<float>(getWidth()) * m_scale),
						static_cast<unsigned int>(static_cast<float>(getHeight()) * m_scale)));
					m_frameCam.zoom(m_scale);
					break;
				}
			}
		}
	}


	IApp::~IApp()
	{
		// Make sure running flag is false
		m_running = false;

		// Remove static pointer
		if (g_pApp == this)
		{
			g_pApp = nullptr;
		}

	}

	void IApp::processArgs(int argc, char* argv[])
	{
		// Handle command line args
		if (argc == 1)	// No arguments
		{
		}
		else
		{
			for (int i = 0; i < argc; ++i)
			{
				// TODO: Process args
			}
		}
	}

	int IApp::run()
	{
		initResources();
		initRenderer();
		m_running = true;

		// Initialization
		initStateFactory();

		gameLoop();
		cleanup();

		m_running = false;

		return m_exitCode;
	}

	bool IApp::isRunning() const
	{
		return m_running;
	}

	unsigned int IApp::getUpdateRate() const
	{
		return static_cast<unsigned int>(1000.0f / static_cast<float>
			(m_time.getTicksPerFrame().asMilliseconds()));
	}

	StateManager & IApp::getStateManager()
	{
		return m_stateManager;
	}

	ResourceManager & IApp::getResourceManager()
	{
		return m_resourceManager;
	}

	ItemGlossary & IApp::getItemGlossary()
	{
		return m_glossary;
	}

	MonsterIndex & IApp::getMonsterIndex()
	{
		return m_bestiary;
	}

	const GameTimer & IApp::getTime() const
	{
		return m_time;
	}

	unsigned int IApp::getWidth() const
	{
		return m_width;
	}

	unsigned int IApp::getHeight() const
	{
		return m_height;
	}

	void IApp::setUpdateRate(unsigned int fps)
	{
		// Logic check update FPS between 1 - 200
		if (fps >= 1 && fps <= 200)
		{
			m_time.setFPS(fps);
		}
	}

	void IApp::setMaxUpdate(sf::Uint32 max)
	{
		// Logic check max updates before frame draw is between 1 - 200
		if (max <= 1 && max >= 200)
		{
			m_maxUpdates = max;
		}
	}

	void IApp::quit(int exitCode)
	{
		// Set an exit code and turn off flag
		m_exitCode = exitCode;
		m_running = false;
	}

	void IApp::initRenderer()
	{
		m_window.create(m_videoMode, m_title, m_windowStyle);

		// Create a RenderTexture which will be drawn to

		m_render.create(m_width, m_height);
		m_window.setVerticalSyncEnabled(false);

		// Create a Sprite of the RenderTexture that is drawn to the window
		m_frame.setTexture(m_render.getTexture());
		m_frame.setScale(m_scale, m_scale);
	}

	void IApp::initResources()
	{
		m_resourceManager.registerHandler(new TResourceHandler<sf::Texture>());
		m_resourceManager.registerHandler(new TResourceHandler<sf::Font>());
	}

	void IApp::cleanup()
	{
		while (m_stateManager.peek().get())
		{
			m_stateManager.pop();
		}
	}

	void IApp::update()
	{
		m_stateManager.logicUpdate();
	}

	void IApp::render()
	{
		m_stateManager.renderUpdate();
		m_render.clear();
		m_stateManager.render(m_render);
		m_render.display();

		m_window.clear(sf::Color::Black);
		m_window.draw(m_frame);
		m_window.display();
	}

}