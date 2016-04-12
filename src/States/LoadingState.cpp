#include "LoadingState.hpp"
#include <cpp3ds/Window/Window.hpp>


namespace DrawAttack {

LoadingState::LoadingState(StateStack& stack, Context& context)
: State(stack, context)
{
	m_text.setFillColor(cpp3ds::Color::Black);
	m_text.setCharacterSize(20);
	m_text.setString("You have been disconnected.\nPress any key to continue.");
}

void LoadingState::renderTopScreen(cpp3ds::Window& window)
{
	window.draw(m_text);
}

void LoadingState::renderBottomScreen(cpp3ds::Window& window)
{
	// Nothing
}

bool LoadingState::update(float delta)
{
	return true;
}

bool LoadingState::processEvent(const cpp3ds::Event& event)
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
