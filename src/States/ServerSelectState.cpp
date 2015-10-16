#include "ServerSelectState.hpp"
#include <cpp3ds/Window/Window.hpp>


namespace DrawAttack {

ServerSelectState::ServerSelectState(StateStack& stack, Context& context)
: State(stack, context)
{
	m_button.setSize(cpp3ds::Vector2f(80, 28));
	m_button.setPosition(230, 200);
	m_button.setFillColor(cpp3ds::Color::Blue);
}

void ServerSelectState::renderTopScreen(cpp3ds::Window& window)
{
	// Nothing
}

void ServerSelectState::renderBottomScreen(cpp3ds::Window& window)
{
	window.draw(m_serverList);
	window.draw(m_button);
}

bool ServerSelectState::update(float delta)
{
	m_serverList.update(delta);
	return true;
}

bool ServerSelectState::processEvent(const cpp3ds::Event& event)
{
	if (event.type == cpp3ds::Event::TouchEnded) {
		if (m_button.getGlobalBounds().contains(event.touch.x, event.touch.y)) {
			std::cout << m_serverList.getSelectedItem().getIp() << std::endl;
			if (getContext().client.connect(m_serverList.getSelectedItem().getIp(),
											m_serverList.getSelectedItem().getPort()) == cpp3ds::Socket::Done)
			{
				requestStackPop();
				requestStackPush(States::Play);
				requestStackPush(States::NameSelect);
			}
		}
	}

	return m_serverList.processEvent(event);
}

} // namespace DrawAttack
