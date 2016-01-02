#include "PickupEvent.hpp"

namespace wv
{
	std::shared_ptr<Animation> PickUpEvent::g_anim = nullptr;
	unsigned int PickUpEvent::g_ref = 0;

	PickUpEvent::PickUpEvent(const sf::Vector2f pos, const std::string& item, const std::string & script)
		:IEvent(), m_itemName(item), m_script(script), m_hitBox(sf::CircleShape(4.0f))
	{
		setPosition(pos);
		m_hitBox.setPosition(pos);

		g_ref++;
		if (!g_anim)
		{
			g_anim = std::make_shared<wv::Animation>("Images/Animations/pickup.png");
			g_anim->addFrame(sf::IntRect(0, 0, 8, 8));
			g_anim->addFrame(sf::IntRect(8, 0, 8, 8));
			g_anim->addFrame(sf::IntRect(16, 0, 8, 8));
			g_anim->addFrame(sf::IntRect(8, 0, 8, 8));
		}

		m_sprite.setAnimation(g_anim);
		m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f);
	}

	PickUpEvent::~PickUpEvent()
	{
		g_ref--;
		if (!g_ref)
		{
			g_anim = nullptr;
		}
	}

	IHitBox & PickUpEvent::getHitBox()
	{
		return m_hitBox;
	}

	void PickUpEvent::collided(Player & p)
	{
		TextBox* tb = p.getTextBox();
		if (tb)
		{
			tb->loadString("You obtained the item " + m_itemName  + "! @p40 @c");
			tb->loadFile(m_script);
			tb->show();
		}
		p.addItem(IApp::instance()->getItemGlossary().getItem(m_itemName));
		done();
	}

	const sf::Drawable & PickUpEvent::getDrawable(const sf::View &)
	{
		return m_sprite;
	}

	void PickUpEvent::update()
	{
		m_sprite.update(IApp::instance()->getTime().getDeltaRenderTime());
	}

}