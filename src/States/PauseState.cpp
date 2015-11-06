#include "PauseState.hpp"
#include <cpp3ds/Window/Window.hpp>


namespace DrawAttack {

PauseState::PauseState(StateStack& stack, Context& context)
: State(stack, context)
{
	m_text.setColor(cpp3ds::Color::Black);
	m_text.setCharacterSize(20);
	m_text.setString("Paused.");
}

void PauseState::renderTopScreen(cpp3ds::Window& window)
{
	window.draw(m_text);
}

void PauseState::renderBottomScreen(cpp3ds::Window& window)
{
	// Nothing
}

bool PauseState::update(float delta)
{
	return true;
}

bool PauseState::processEvent(const cpp3ds::Event& event)
{
	// If any key is pressed, trigger the next screen
	if (event.type == cpp3ds::Event::KeyPressed)
	{
		requestStackPop();
	}

	// Don't propagate event processing
	return false;
}

} // namespace DrawAttack
