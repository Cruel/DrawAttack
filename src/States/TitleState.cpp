#include "TitleState.hpp"
#include <cpp3ds/Window/Window.hpp>


namespace DrawAttack {

TitleState::TitleState(StateStack& stack, Context& context)
: State(stack, context)
, m_text()
, m_showText(true)
, m_textEffectTime(0)
{
	m_textTitle.setCharacterSize(50);
	m_textTitle.setStyle(cpp3ds::Text::Bold);
	m_textTitle.setFillColor(cpp3ds::Color::Black);
	m_textTitle.setString(_("DrawAttack"));
	m_textTitle.setPosition(std::floor(200.f - m_textTitle.getLocalBounds().width / 2), 10.f);

	m_text.setCharacterSize(12);
	m_text.setFillColor(cpp3ds::Color::Black);
	m_text.setString(_("Press any key to start. SELECT to exit."));
	m_text.setPosition(std::floor(200.f - m_text.getLocalBounds().width / 2), 150.f);
}

void TitleState::renderTopScreen(cpp3ds::Window& window)
{
	window.draw(m_textTitle);
	if (m_showText)
		window.draw(m_text);
}

void TitleState::renderBottomScreen(cpp3ds::Window& window)
{
	// Nothing
}

bool TitleState::update(float delta)
{
	m_textEffectTime += delta;

	if (m_textEffectTime >= 0.5f)
	{
		m_showText = !m_showText;
		m_textEffectTime = 0;
	}

	return true;
}

bool TitleState::processEvent(const cpp3ds::Event& event)
{
	// If any key is pressed, trigger the next screen
	if (event.type == cpp3ds::Event::KeyPressed)
	{
		if (event.key.code == cpp3ds::Keyboard::Select) {
			requestStackClear();
			return true;
		}

		if (cpp3ds::Service::isEnabled(cpp3ds::Network)) {
			requestStackPop();
			requestStackPush(States::ServerSelect);
		} else {
			getContext().transition.message = _("No internet connection.\nConnect to internet and retry.");
			requestStackPush(States::TransitionMessage);
		}
	}

	return true;
}

} // namespace DrawAttack
