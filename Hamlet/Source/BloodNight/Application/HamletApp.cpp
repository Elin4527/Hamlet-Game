#include "HamletApp.hpp"
#include "../Items/Fist.hpp"
#include "../Items/Sword.hpp"
#include "../States/MainMenuState.hpp"
#include "../Enemies/EvilHamlet.hpp"
#include "../Enemies/Ghost.hpp"
#include "../Enemies/Polonius.hpp"
#include "../Enemies/Laertes.hpp"
#include "../Enemies/Claudius.hpp"
#include <string>
#include <iostream>

HamletApp::HamletApp()
	:IApp("Hamlet", 640, 480, 1.0f, sf::Style::Close)
{
}

void HamletApp::initResources()
{
	wv::IApp::initResources();

	// Load the items
	getItemGlossary().addItem(std::make_unique<Fist>());
	getItemGlossary().addItem(std::make_unique<Sword>());

	// Load the monsters
	wv::IApp::instance()->getMonsterIndex().addEnemy(std::make_unique<EvilHamlet>());
	wv::IApp::instance()->getMonsterIndex().addEnemy(std::make_unique<Ghost>());
	wv::IApp::instance()->getMonsterIndex().addEnemy(std::make_unique<Polonius>());
	wv::IApp::instance()->getMonsterIndex().addEnemy(std::make_unique<Laertes>());
	wv::IApp::instance()->getMonsterIndex().addEnemy(std::make_unique <Claudius>());

}

void HamletApp::initStateFactory()
{
	getStateManager().push(std::shared_ptr<wv::IState>(new MainMenuState), wv::modality::EXCLUSIVE);

}
