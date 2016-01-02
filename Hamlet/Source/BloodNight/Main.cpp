#include "Application\HamletApp.hpp"

int main(int argc, char* argv[])
{
	HamletApp game;
	game.processArgs(argc, argv);
	return game.run();
}