#pragma once

#include "../../WildVEngine/WildVEngine.hpp"

class RSlash : public wv::IAbility
{
	static std::shared_ptr<wv::Animation> g_anim;
	static unsigned int g_ref;
public:
	RSlash(sf::Time cd);
	virtual ~RSlash();
	virtual void use(wv::Battler & p) override;
	virtual wv::IAbility* clone() const override;
protected:
	virtual void effect(wv::Battler & user, wv::Battler& target) override;
};