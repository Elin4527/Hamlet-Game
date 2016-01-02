#pragma once
#include "IObject.hpp"
#include <queue>
#include "../Interfaces/TResource.hpp"

namespace wv
{
	class TileMap;
	class TextBox : public LogicComponent, public RenderComponent
	{
		TileMap* m_tilemap;
		sf::RenderTexture m_render;
		sf::Sprite m_drawable;
		sf::Sprite m_background;
		TResource<sf::Texture> m_texture;
		TResource<sf::Font> m_font;
		sf::Vector2u m_textAreaDimensions;
		sf::Vector2f m_location;
		std::vector<sf::Text> m_text;
		std::queue<std::string> m_words;
		size_t m_currRow;
		size_t m_cursor;
		std::string m_currLine;
		sf::Time m_hidingTime;
		unsigned int m_count;
		unsigned int m_delay;
		unsigned int m_padding;
		unsigned int m_charSize;
		bool m_instant;
		bool m_scrolling;
		bool m_visible;
		bool m_full;
		bool m_finish;

	public:
		TextBox(TileMap* tMap, const std::string& font, const std::string& texture, unsigned int charSize = 30,
			unsigned int padding = 8, bool visible = true, bool instant = false, bool scroll = false);
		virtual ~TextBox();

		void show();
		void hide();
		void close();
		bool gameOver() const;

		void loadString(const std::string& message);
		void loadFile(const std::string& file);
		void setLocation(const sf::Vector2f& pos);
		bool isVisible() const;

		virtual void setColor(const sf::Color & c) override;

		virtual void fixedUpdate() override;
		virtual bool handleInput(sf::Event& e) override;
		virtual sf::Drawable& getDrawable(const sf::View& view) override;
	private:
		void fillWords();
		void nextLetter();
		void nextLine();
		void processCommand(const std::string& c);
		void clear();
	};

}