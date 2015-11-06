#include "PauseState.hpp"
#include <cpp3ds/Window/Window.hpp>


namespace DrawAttack {

PauseState::PauseState(StateStack& stack, Context& context)
: State(stack, context)
{
	m_overlay.setSize(cpp3ds::Vector2f(400.f, 240.f));
	m_overlay.setFillColor(cpp3ds::Color(0, 0, 0, 220));

	m_texture.loadFromFile("images/button.9.png");

	m_buttonResume.setTexture(&m_texture);
	m_buttonResume.setString(_("Resume"));
	m_buttonResume.setActiveColor(cpp3ds::Color(255, 255, 255, 180));
	m_buttonDisconnect.setTexture(&m_texture);
	m_buttonDisconnect.setString(_("Disconnect"));
	m_buttonDisconnect.setActiveColor(cpp3ds::Color(255, 255, 255, 180));
	m_buttonQuit.setTexture(&m_texture);
	m_buttonQuit.setString(_("Quit"));
	m_buttonQuit.setActiveColor(cpp3ds::Color(255, 255, 255, 180));

	m_buttonResume.setPosition(40.f, 40.f);
	m_buttonDisconnect.setPosition(40.f, 80.f);
	m_buttonQuit.setPosition(40.f, 120.f);

	m_buttonResume.onClick([this]{
		requestStackPop();
	});
	m_buttonDisconnect.onClick([this]{
		requestStackClear();
		requestStackPush(States::ServerSelect);
	});
	m_buttonQuit.onClick([this]{
		requestStackClear();
		requestStackPush(States::Title);
	});
}

void PauseState::renderTopScreen(cpp3ds::Window& window)
{
	window.draw(m_overlay);
}

void PauseState::renderBottomScreen(cpp3ds::Window& window)
{
	window.draw(m_overlay);

	window.draw(m_buttonResume);
	window.draw(m_buttonDisconnect);
	window.draw(m_buttonQuit);
}

bool PauseState::update(float delta)
{
	return true;
}

bool PauseState::processEvent(const cpp3ds::Event& event)
{
	m_buttonResume.processEvent(event);
	m_buttonDisconnect.processEvent(event);
	m_buttonQuit.processEvent(event);

	if (event.type == cpp3ds::Event::KeyPressed)
	{
		if (event.key.code == cpp3ds::Keyboard::Start) {
			requestStackPop();
		}
	}

	// Don't propagate event processing
	return false;
}

} // namespace DrawAttack
