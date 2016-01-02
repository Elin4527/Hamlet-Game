#include "TextBox.hpp"
#include <iostream>
#include <fstream>
#include "../Event/MapChangeEvent.hpp"
#include "../Event/PickupEvent.hpp"
#include "../Event/TextEvent.hpp"
#include "TileMap.hpp"

namespace wv
{

	TextBox::TextBox(TileMap* tmap, const std::string& f, const std::string& t, unsigned int charSize, unsigned int padding, bool visible, bool instant, bool scroll)
		:m_font(f), m_texture(t), m_charSize(charSize), m_visible(visible), m_padding(padding), m_count(0), m_instant(instant), m_scrolling(scroll), m_full(false), m_tilemap(tmap), m_finish(false)
	{
		m_background.setTexture(m_texture.getResource());
		m_textAreaDimensions.x = m_background.getLocalBounds().width - padding * 2;
		m_textAreaDimensions.y = m_background.getLocalBounds().height - padding * 2;
		
		// Get number of rows that will fit in the text area
		unsigned int size = static_cast<unsigned int>(m_textAreaDimensions.y / m_font.getResource().getLineSpacing(charSize));
		

		// Initialize the text rows
		for (unsigned int i = 0; i < size; i++)
		{
			m_text.emplace_back("", m_font.getResource(), charSize);
			m_text[i].setPosition(padding, padding + static_cast<unsigned int>(i * (m_font.getResource().getLineSpacing(charSize))));
		}

		m_render.create(m_background.getLocalBounds().width, m_background.getLocalBounds().height);
		m_drawable.setTexture(m_render.getTexture());
	}

	TextBox::~TextBox()
	{}

	void TextBox::show()
	{
		m_visible = true;
	}

	void TextBox::hide()
	{
		m_visible = false;
	}

	void TextBox::close()
	{
		m_visible = false;
		clear();
	}

	bool TextBox::gameOver() const
	{
		return m_finish;
	}

	bool TextBox::isVisible() const
	{
		return m_visible;
	}

	void TextBox::loadFile(const std::string& file)
	{
		std::ifstream input;
		input.open(file);
		if (!input.is_open())
		{
			std::cout << "Could not open script: " + file << "!"<< std::endl;
			return;
		}

		std::string message;
		while (std::getline(input, message))
		{
			loadString(message);
		}
		
	}

	void TextBox::loadString(const std::string & message)
	{
		int begin = 0;

		// Seperate into words
		for (size_t i = 0; i < message.size(); i++)
		{
			if (message[i] == ' ')
			{
				m_words.push(message.substr(begin, i - begin));
				begin = i + 1;
			}
		}
		m_words.push(message.substr(begin));
		if (m_instant && !m_full)
		{
			fillWords();
		}
 	}

	void TextBox::setLocation(const sf::Vector2f& pos)
	{
		m_location = pos;
	}

	void TextBox::fixedUpdate()
	{
		if (m_visible && !m_full)
		{	
			if (m_delay > 0)
			{
				m_delay--;
			}
			else
			{
				nextLetter();
			}

		}
		else if (m_hidingTime > sf::Time::Zero)
		{
			m_hidingTime -= IApp::instance()->getTime().getTicksPerFrame();
			if (m_hidingTime <= sf::Time::Zero)
			{
				m_hidingTime = sf::Time::Zero;
				show();
			}
		}
	}

	bool TextBox::handleInput(sf::Event & e)
	{
		if (m_visible)
		{
			if (e.type == sf::Event::KeyPressed && (e.key.code == sf::Keyboard::Space || e.key.code == sf::Keyboard::Return))
			{	
				if (m_words.empty())
				{
					close();
				}
				else if (m_delay > 0)
				{
					m_delay = 0;
				}
				else if (m_full)
				{
					nextLetter();
				}
				else
				{
					fillWords();
				}
				return true;
			}
		}
		return false;
	}

	sf::Drawable& TextBox::getDrawable(const sf::View& view)
	{
		m_render.clear(sf::Color::Transparent);

		if (m_visible)
		{

			// Set position from view so that relative screen location is the same
			setPosition(view.getCenter() - view.getSize() / 2.0f + m_location);

			m_render.draw(m_background);
			for (auto& t : m_text)
			{
				m_render.draw(t);
			}
		}
		m_render.display();
		return m_drawable;
	}

	void TextBox::nextLetter()
	{
		if (m_full)
		{
			nextLine();
		}
		if (!m_words.empty())
		{
			std::string word = m_words.front();
			std::string line = m_currLine;
			if (word.at(0) == '@')
			{
				processCommand(word);
				m_words.pop();
				return;
			}
			m_cursor++;

			if (m_currLine != "")
			{
				line += " " + word.substr(0, m_cursor);
			}
			else
			{
				line = word.substr(0, m_cursor);
			}

			sf::Text temp(line, m_font.getResource(), m_charSize);

			if (temp.getLocalBounds().width >= m_textAreaDimensions.x)
			{
				// If the current line isn't empty just remove the current word from that line
				if (!m_currLine.empty())
				{
					// Go to the next line, first set the old line
					m_text[m_currRow].setString(m_currLine);

					// Start with an empty string and start printing from here
					m_currLine = "";
					if (m_currRow == m_text.size() - 1)
					{
						m_full = true;
						return;
					}
					nextLine();
					m_text[m_currRow].setString(word.substr(0, m_cursor));
				}
				// Otherwise the word was too long to fit on a single line, cut it
				else
				{
					m_words.front() = m_words.front().substr(m_cursor);
					m_cursor = 0;
					if (m_currRow == m_text.size() - 1)
					{
						m_full = true;
						return;
					}
					nextLine();
				}
			}
			else
			{
				m_text[m_currRow].setString(line);
			}
			if (m_cursor == word.size())
			{
				m_words.pop();
				m_currLine = m_text[m_currRow].getString();
				m_cursor = 0;
			}
		}
	}

	void TextBox::fillWords()
	{

		if (m_full)
		{
			nextLine();
		}
		while (!m_words.empty())
		{
			std::string word = m_words.front();
			std::string line = m_currLine;

			if (word.at(0) == '@')
			{
				processCommand(word);
				m_words.pop();
				return;
			}
			if (line != "")
			{
				line += " " + word;
			}
			else
			{
				line = word;
			}

			sf::Text temp(line, m_font.getResource(), m_charSize);

			if (temp.getLocalBounds().width >= m_textAreaDimensions.x)
			{
				if (!m_currLine.empty())
				{
					m_text[m_currRow].setString(m_currLine);
					m_currLine = "";

					if (m_currRow == m_text.size() - 1)
					{
						m_full = true;
						break;
					}
					nextLine();
					temp.setString(word);
				}
				// If the current line is empty cut off the word cause its too long
				if (temp.getLocalBounds().width >= m_textAreaDimensions.x && m_currLine.empty())
				{
					int i = word.size() - 1;
					for (; i > 0; i--)
					{
						temp.setString(word.substr(0, i));
						if (temp.getLocalBounds().width < m_textAreaDimensions.x)
						{
							m_text[m_currRow].setString(word.substr(0, i));
							break;
						}
					}
					m_words.front() = word.substr(i);
					m_currLine = "";

					if (m_currRow == m_text.size() - 1)
					{
						m_full = true;
						break;
					}
					nextLine();

				}
			}

			else
			{
				m_text[m_currRow].setString(line);
				m_words.pop();

			}
			
			m_currLine = m_text[m_currRow].getString();
			m_cursor = 0;
		}
	}

	void TextBox::nextLine()
	{
		m_full = false;
		m_currLine = "";
		if (!m_full && m_currRow < m_text.size() - 1)
		{
			m_currRow++;
		}
		else
		{
			if (m_scrolling)
			{
				for (size_t i = 0; i < m_text.size() - 1; i++)
				{
					m_text[i].setString(m_text[i + 1].getString());
				}
			}
			else
			{
				clear();
			}
		}
	}

	void TextBox::processCommand(const std::string & c)
	{
		switch (c.at(1))
		{
			// Clear command
			case 'c':
				clear();
				break;
			// Event creation command
			case 'e':
			{
				size_t start = 3;
				size_t delim = c.find('~', start);
				int x = std::atoi(c.substr(start, delim - start).c_str());
				start = delim + 1;
				delim = c.find('~', start);
				int y = std::atoi(c.substr(start, delim - start).c_str());
				start = delim + 1;
				sf::Vector2f pos((x + 0.5f) * m_tilemap->getTileSize(), (y + 0.5f) * m_tilemap->getTileSize());
				switch (c.at(2))
				{
					// Pickup event
					case 'p':
					{
						delim = c.find('~', start);
						std::string item = c.substr(start, delim - start);
						start = delim + 1;
						std::string script = c.substr(start);
						m_tilemap->addEvent(std::make_unique<PickUpEvent>(pos, item, script));
						break;
					}
					// Map change event
					case 'm':
					{
						std::string file = c.substr(start);
						m_tilemap->addEvent(std::make_unique<MapChangeEvent>(pos, file));
						break;
					}
					case 't':
					{
						std::string file = c.substr(start);
						m_tilemap->addEvent(std::make_unique<TextEvent>(pos, file));
						break;
					}
					default:
						break;
				}

				break;
			}
			// Pause command
			case 'p':
				m_delay = std::atoi(c.substr(2).c_str());
				break;
			// Hide for a set amount of time command
			case 'h':
				hide();
				m_hidingTime = sf::seconds(std::atof(c.substr(2).c_str()));
				break;
			// Quit command
			case 'q':
				close();
				break;
			// Newline command
			case 'n':
				nextLine();
				break;
			// Game is over finished
			case 'f':
				m_finish = true;
				break;
			// Wait for user input by treating as full
			case 'w':
				m_full = true;
				break;
			default:
				break;
		}
	}

	void TextBox::clear()
	{
		m_currLine = "";
		for (auto& t : m_text)
		{
			t.setString("");
		}
		m_currRow = 0;
	}

	void TextBox::setColor(const sf::Color & c)
	{
		for (auto& t : m_text)
		{
			t.setColor(c);
		}
	}

}