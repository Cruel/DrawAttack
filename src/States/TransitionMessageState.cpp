#include "TransitionMessageState.hpp"
#include <cpp3ds/Window/Window.hpp>


namespace DrawAttack {

TransitionMessageState::TransitionMessageState(StateStack& stack, Context& context)
: State(stack, context)
{
	m_text.setFillColor(cpp3ds::Color::White);
	m_text.setCharacterSize(14);
	m_text.setPosition(20.f, 60.f);
	m_text.setString(getContext().transition.message + _("\nPress A to continue."));

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
	if (event.type == cpp3ds::Event::KeyPressed && event.key.code == cpp3ds::Keyboard::A)
	{
		requestStackPop();

		States::ID newState = getContext().transition.nextStateID;
		if (newState != States::None) {
			requestStackClear();
			requestStackPush(newState);
		}

		getContext().transition.message.clear();
		getContext().transition.nextStateID = States::None;
		return false;
	}

	return true;
}

} // namespace DrawAttack
