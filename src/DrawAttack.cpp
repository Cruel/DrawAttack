#include "DrawAttack.hpp"
#include "States/TitleState.hpp"
#include "States/ServerSelectState.hpp"
#include "States/PlayState.hpp"
#include "States/PauseState.hpp"
#include "States/TransitionMessageState.hpp"
#include "States/NameSelectState.hpp"
#include "States/LoadingState.hpp"
#include "Notification.hpp"


using namespace cpp3ds;
using namespace TweenEngine;

namespace DrawAttack {


DrawAttack::DrawAttack()
: m_stateStack(State::Context(m_client, m_name, m_data, m_transition))
{
	textFPS.setColor(Color::Blue);

	m_stateStack.registerState<TitleState>(States::Title);
	m_stateStack.registerState<ServerSelectState>(States::ServerSelect);
	m_stateStack.registerState<NameSelectState>(States::NameSelect);
	m_stateStack.registerState<PlayState>(States::Play);
	m_stateStack.registerState<PauseState>(States::Pause);
	m_stateStack.registerState<TransitionMessageState>(States::TransitionMessage);
	m_stateStack.registerState<LoadingState>(States::Loading);
	m_stateStack.pushState(States::ServerSelect);
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

	static int i;
	if (i++ % 10 == 0) {
		textFPS.setString(_("%.1f fps", 1.f / delta));
		textFPS.setPosition(395 - textFPS.getGlobalBounds().width, 200);
	}

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
	window.draw(textFPS);
	for (auto& notification : Notification::notifications)
		window.draw(*notification);
}

void DrawAttack::renderBottomScreen(Window& window)
{
	window.clear(Color(100, 100, 220));
	m_stateStack.renderBottomScreen(window);
}


} // namespace DrawAttack
