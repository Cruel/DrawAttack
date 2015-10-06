#include <iostream>
#include <cpp3ds/Graphics.hpp>
#include "DrawAttack.hpp"

using namespace cpp3ds;

int main(int argc, char** argv) {
	Console::initialize();
	if (!Service::enable(NETWORK))
		std::cout << "SERVICE FAILURE!" << std::endl;
	DrawAttack::DrawAttack game;
	game.console(TopScreen); // Console for reading stdout
	game.run();
	return 0;
}
