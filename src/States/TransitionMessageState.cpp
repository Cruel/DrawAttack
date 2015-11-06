#include "TransitionMessageState.hpp"
#include <cpp3ds/Window/Window.hpp>


namespace DrawAttack {

TransitionMessageState::TransitionMessageState(StateStack& stack, Context& context)
: State(stack, context)
{
	m_text.setColor(cpp3ds::Color::White);
	m_text.setCharacterSize(20);
	m_text.setPosition(20.f, 60.f);
	m_text.setString(_("%s\nPress any key to continue.", getContext().transition.message.c_str()));

	m_rectangle.setSize(cpp3ds::Vector2f(400, 240));
	m_rectangle.setFillColor(cpp3ds::Color(0, 0, 0, 200));
}

void TransitionMessageState::renderTopScreen(cpp3ds::Window& window)
{
	window.draw(m_rectangle);
	window.draw(m_text);
}

void TransitionMessageState::renderBottomScreen(cpp3ds::Window& window)
{
	window.draw(m_rectangle);
}

bool TransitionMessageState::update(float delta)
{
	return true;
}

bool TransitionMessageState::processEvent(const cpp3ds::Event& event)
{
	// If any key is pressed, trigger the next screen
	if (event.type == cpp3ds::Event::KeyPressed)
	{
		requestStackPop();

		States::ID newState = getContext().transition.nextStateID;
		if (newState != States::None) {
			requestStackPop();
			requestStackPush(newState);
		}

		getContext().transition.message.clear();
		getContext().transition.nextStateID = States::None;
		return false;
	}

	return true;
}

} // namespace DrawAttack
