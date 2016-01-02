#include "PlayerGUI.hpp"
#include "../Abilities/AbilitySet.hpp"
#include <sstream>
#include <iomanip>

namespace wv
{
	PlayerGUI::PlayerGUI(Player& p)
		:m_player(p), m_health("Images/GUI/health.png"), m_skill("Images/GUI/skillBack.png"), m_item("Images/GUI/itemBack.png"),
		m_font("Font/calibri.ttf")
	{
		m_render.create(IApp::instance()->getWidth(), IApp::instance()->getHeight());
		m_drawable.setTexture(m_render.getTexture());
	}

	void wv::PlayerGUI::setColor(const sf::Color & c)
	{
	}

	const sf::Drawable & wv::PlayerGUI::getDrawable(const sf::View & v)
	{
		sf::Vector2f dim = v.getSize();
		sf::Vector2f center = v.getCenter();
		sf::FloatRect screenRect(sf::Vector2f(center.x - (dim.x / 2.0f), center.y - (dim.y) / 2), dim);
		m_render.clear(sf::Color::Transparent);

		setPosition(screenRect.left, screenRect.top);

		unsigned int end = m_player.getMaxHealth();
		unsigned int currHealth = m_player.getHealth();

		for (unsigned int i = 0; i < end; i++)
		{
			sf::Sprite bar(m_health.getResource());
			bar.setPosition((bar.getLocalBounds().width + 10) * i + 20, 20);
			if (i >= currHealth)
			{
				bar.setColor(sf::Color(50, 50, 50));
			}
			m_render.draw(bar);
		}

		end = m_player.getMaxInventorySize();
		for (int i = 0; i < end; i++)
		{
			sf::Sprite back(m_item.getResource());
			back.setPosition(20, i * (back.getLocalBounds().height + 10) + 64);
			if (i != m_player.m_selectedItem)
			{
				back.setColor(sf::Color(50, 50, 50));
			}
			m_render.draw(back);

			if (i < m_player.m_inventory.size())
			{
				sf::Sprite item(m_player.m_inventory[i]->getTexture());
				int xDif = back.getLocalBounds().width - item.getLocalBounds().width;
				int yDif = back.getLocalBounds().height - item.getLocalBounds().height;
				item.setPosition(20 + xDif / 2, i * (back.getLocalBounds().height + 10) + 64 + yDif / 2);
				if (i != m_player.m_selectedItem)
				{
					item.setColor(sf::Color(50, 50, 50));
				}
				m_render.draw(item);
			}
		}

		static std::string keys[] = { "Q", "W", "E", "R" };
		if (m_player.getAbilities())
		{
			end = m_player.getAbilities()->size();
			for (int i = 0; i < end; i++)
			{
				sf::Sprite back(m_skill.getResource());
				back.setPosition(IApp::instance()->getWidth() - 20 - (end - i) * (back.getLocalBounds().width + 10), 20);
				auto ptr = m_player.getAbilities()->getAbility(i);

				sf::Text k(keys[i], m_font.getResource(), 12);

				k.setPosition(std::roundf(IApp::instance()->getWidth() - 20 - (end - i) * (back.getLocalBounds().width + 10) + ((back.getLocalBounds().width - k.getLocalBounds().width) / 2)), 20 + back.getLocalBounds().height);
				k.setColor(sf::Color(100, 100, 255));
				if (ptr)
				{
					sf::Sprite skill(ptr->getTexture());
					int xDif = back.getLocalBounds().width - skill.getLocalBounds().width;
					int yDif = back.getLocalBounds().height - skill.getLocalBounds().height;
					skill.setPosition(IApp::instance()->getWidth() - 20 - (end - i) * (back.getLocalBounds().width + 10) + xDif / 2, 20 + yDif / 2);

					if (ptr->getRemainingTime() != sf::Time::Zero)
					{
						// Dim when on cd
						back.setColor(sf::Color(50, 50, 50));
						skill.setColor(sf::Color(50, 50, 50));

						// Display remaining cd
						std::ostringstream str;
						str << std::fixed << std::setprecision(1) << ptr->getRemainingTime().asSeconds();
						sf::Text cd(str.str(), m_font.getResource(), 12);
						cd.setOrigin(cd.getLocalBounds().width / 2.0f, cd.getLocalBounds().height);
						cd.setPosition(IApp::instance()->getWidth() - 20 - (end - i) * (back.getLocalBounds().width + 10) + back.getLocalBounds().width / 2.0f, 20 + back.getLocalBounds().height / 2.0f);

						// Draw everything
						m_render.draw(back);
						m_render.draw(skill);
						m_render.draw(cd);
						m_render.draw(k);
					}
					else
					{
						m_render.draw(back);
						m_render.draw(skill);
						m_render.draw(k);
					}
				}
				else
				{
					back.setColor(sf::Color(50, 50, 50));
					m_render.draw(back);
					m_render.draw(k);
				}
			}
		}
		std::ostringstream fps;
		fps << "FPS: " << std::roundf(1.0f / IApp::instance()->getTime().getDeltaRenderTime().asSeconds());
		sf::Text f;
		f.setFont(m_font.getResource());
		f.setString(fps.str());
		f.setCharacterSize(12);
		f.setColor(sf::Color::Green);
		f.setPosition(20, IApp::instance()->getHeight() - m_font.getResource().getLineSpacing(12) - 8);

		m_render.draw(f);
		m_render.display();
		return m_drawable;
	}

}