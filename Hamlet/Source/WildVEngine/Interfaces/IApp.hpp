#pragma once

#include <SFML\Graphics.hpp>
#include "..\Classes\StateManager.hpp"
#include "..\Classes\GameTimer.hpp"
#include "..\Classes\ResourceManager.hpp"
#include "..\Classes\ItemGlossary.hpp"
#include "..\Classes\MonsterIndex.hpp"

namespace wv
{
	// Class that represents the whole game application
	// Based on: https://code.google.com/p/gqe/

	class IApp
	{
		// Class vars
		static IApp* g_pApp;

		// Member vars
		ResourceManager m_resourceManager;
		StateManager m_stateManager;
		GameTimer m_time;
		ItemGlossary m_glossary;
		MonsterIndex m_bestiary;

		// Rendering the game
		sf::RenderWindow m_window;
		sf::VideoMode m_videoMode;
		sf::RenderTexture m_render;
		sf::View m_frameCam;
		sf::Sprite m_frame;
		std::string m_title;
		sf::Uint32 m_maxUpdates;
		
		unsigned long m_windowStyle;
		unsigned int m_width;
		unsigned int m_height;
		int m_exitCode;
		float m_scale;
		bool m_running;

	public:
		virtual ~IApp();

		virtual void processArgs(int argc, char* argv[]);	// command line args
		int run();
		void quit(int exitCode);

		// Getters
		bool isRunning() const;
		unsigned int getWidth() const;
		unsigned int getHeight() const;
		unsigned int getUpdateRate() const;
		StateManager& getStateManager();
		ResourceManager& getResourceManager();
		ItemGlossary& getItemGlossary();
		MonsterIndex& getMonsterIndex();
		const GameTimer& getTime() const;

		// Setters
		void setUpdateRate(unsigned int rate);
		void setMaxUpdate(sf::Uint32 maxUpdates);

		// Singleton
		static IApp* instance()
		{
			return g_pApp;
		}
	protected:
		// Protected constructor for inheritance
		IApp(const std::string& title, unsigned int width, unsigned int height, float scale = 1.0f, unsigned long winStyle = sf::Style::Default);

		virtual void initStateFactory() = 0;
		virtual void initRenderer();
		virtual void initResources();
	private:
		void gameLoop();
		
		void cleanup();
		
		void processInput();
		void update();
		void render();
	};
}