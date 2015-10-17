#include "ServerSelectState.hpp"
#include <cpp3ds/Window/Window.hpp>


namespace DrawAttack {

ServerSelectState::ServerSelectState(StateStack& stack, Context& context)
: State(stack, context)
{
	m_button.setString(_("Connect"));
	m_button.setColor(cpp3ds::Color::Blue);
	m_button.setActiveColor(cpp3ds::Color::Red);
	m_button.setTextColor(cpp3ds::Color::Red);
	m_button.setTextActiveColor(cpp3ds::Color::Blue);

	m_button.onClick([this]{
		ServerListItem serverItem = m_serverList.getSelectedItem();
		std::cout << serverItem.getIp() << std::endl;
		if (getContext().client.connect(serverItem.getIp(),
										serverItem.getPort()) == cpp3ds::Socket::Done)
		{
			requestStackPop();
			requestStackPush(States::Play);
			requestStackPush(States::NameSelect);
		}
	});
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
	m_button.setPosition(315 - m_button.getSize().x, 235 - m_button.getSize().y);
	m_button.update(delta);

	m_serverList.update(delta);

	return true;
}

bool ServerSelectState::processEvent(const cpp3ds::Event& event)
{
	m_button.processEvent(event);

	return m_serverList.processEvent(event);
}

} // namespace DrawAttack
