#ifndef DRAWATTACK_DRAWATTACK_H
#define DRAWATTACK_DRAWATTACK_H

#include <cpp3ds/Graphics.hpp>
#include <cpp3ds/Network.hpp>
#include <TweenEngine/TweenManager.h>
#include "DrawingBoard.hpp"
#include "Keyboard/Keyboard.hpp"
#include "States/StateStack.hpp"
#include "Network/Client.hpp"


namespace DrawAttack {

class DrawAttack: public cpp3ds::Game {
public:
	DrawAttack();
	~DrawAttack();
	void update(float delta);
	void processEvent(cpp3ds::Event& event);
	void renderTopScreen(cpp3ds::Window& window);
	void renderBottomScreen(cpp3ds::Window& window);

private:
	StateStack m_stateStack;

	// Shared State context variables
	Client m_client;
	cpp3ds::String m_name;
	cpp3ds::String m_data;
	State::TransitionData m_transition;
	cpp3ds::Color m_color;
};

}

#endif // DRAWATTACK_DRAWATTACK_H
