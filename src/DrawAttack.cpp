#include "DrawAttack.hpp"
#include "States/TitleState.hpp"
#include "States/ServerSelectState.hpp"
#include "States/PlayState.hpp"
#include "States/PauseState.hpp"
#include "States/TransitionMessageState.hpp"
#include "States/NameSelectState.hpp"
#include "States/LoadingState.hpp"
#include "Notification.hpp"
#include "States/ColorPickerState.hpp"


using namespace cpp3ds;
using namespace TweenEngine;

namespace DrawAttack {


DrawAttack::DrawAttack()
: m_color(cpp3ds::Color::Black)
, m_stateStack(State::Context(m_client, m_name, m_data, m_transition, m_color))
{
	m_stateStack.registerState<TitleState>(States::Title);
	m_stateStack.registerState<ServerSelectState>(States::ServerSelect);
	m_stateStack.registerState<NameSelectState>(States::NameSelect);
	m_stateStack.registerState<PlayState>(States::Play);
	m_stateStack.registerState<PauseState>(States::Pause);
	m_stateStack.registerState<TransitionMessageState>(States::TransitionMessage);
	m_stateStack.registerState<LoadingState>(States::Loading);
	m_stateStack.registerState<ColorPickerState>(States::ColorPicker);
	m_stateStack.pushState(States::Title);
}

DrawAttack::~DrawAttack()
{
	// Destructor called when game exits
}

void DrawAttack::update(float delta)
{
	// Need to update before checking if empty
	m_stateStack.update(delta);
	if (m_stateStack.isEmpty())
		exit();

	Notification::update(delta);
}

void DrawAttack::processEvent(Event& event)
{
	m_stateStack.processEvent(event);
}

void DrawAttack::renderTopScreen(Window& window)
{
	window.clear(Color::White);
	m_stateStack.renderTopScreen(window);
	window.setView(window.getDefaultView());
	for (auto& notification : Notification::notifications)
		window.draw(*notification);
}

void DrawAttack::renderBottomScreen(Window& window)
{
	window.clear(Color(100, 100, 220));
	m_stateStack.renderBottomScreen(window);
}


} // namespace DrawAttack
