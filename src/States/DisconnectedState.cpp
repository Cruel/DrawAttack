#include "DisconnectedState.hpp"
#include <cpp3ds/Window/Window.hpp>


namespace DrawAttack {

DisconnectedState::DisconnectedState(StateStack& stack, Context& context)
: State(stack, context)
{
	m_text.setColor(cpp3ds::Color::Black);
	m_text.setCharacterSize(20);
	m_text.setString("You have been disconnected.\nPress any key to continue.");
}

void DisconnectedState::renderTopScreen(cpp3ds::Window& window)
{
	window.draw(m_text);
}

void DisconnectedState::renderBottomScreen(cpp3ds::Window& window)
{
	// Nothing
}

bool DisconnectedState::update(float delta)
{
	return true;
}

bool DisconnectedState::processEvent(const cpp3ds::Event& event)
{
	// If any key is pressed, trigger the next screen
	if (event.type == cpp3ds::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(States::ServerSelect);
	}

	return true;
}

} // namespace DrawAttack
