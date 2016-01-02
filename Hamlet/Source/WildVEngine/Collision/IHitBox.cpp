#include "IHitBox.hpp"

namespace wv
{


	IHitBox::IHitBox(bool vis)
		:m_visible(vis)
	{
	}


	IHitBox::~IHitBox() {}


	void IHitBox::setVisibility(bool vis)
	{
		m_visible = vis;
	}

	bool IHitBox::isVisible() const
	{
		return m_visible;
	}
}