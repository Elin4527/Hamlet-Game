#pragma once

#include "..\..\WildVEngine\Interfaces\IApp.hpp"
#include "..\Items\Fist.hpp"

class HamletApp : public wv::IApp
{
public:
	HamletApp();
private:
	virtual void initResources() override;
	virtual void initStateFactory() override;
};