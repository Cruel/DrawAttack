#include <iostream>
#include <cpp3ds/Graphics.hpp>
#include "DrawAttack.hpp"

using namespace cpp3ds;

int main(int argc, char** argv) {
//	Console::enable(TopScreen, cpp3ds::Color::Black); // Console for reading stdout
//	Console::enableBasic(TopScreen); // Console for reading stdout
	if (!Service::enable(Network))
		std::cout << "SERVICE FAILURE!" << std::endl;
	if (!Service::enable(Microphone))
		std::cout << "NO MIC!" << std::endl;
	DrawAttack::DrawAttack game;
	game.run();
	return 0;
}
