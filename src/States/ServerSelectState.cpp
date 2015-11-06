#include "ServerSelectState.hpp"
#include "../Notification.hpp"
#include <TweenEngine/Tween.h>

namespace DrawAttack {

ServerSelectState::ServerSelectState(StateStack& stack, Context& context)
: State(stack, context)
, m_menuVisible(false)
{
	m_caption.setString(_("Select a server:"));
	m_caption.setColor(cpp3ds::Color::Black);
	m_caption.setPosition(10.f, 100.f);

	m_buttonTexture.loadFromFile("images/button.9.png");
	m_iconFont.loadFromFile("fonts/fontawesome.ttf");

	m_button.setTexture(&m_buttonTexture);
	m_button.setString(_("Connect"));
	m_button.getText().setCharacterSize(18);
	m_button.setTextOffset(cpp3ds::Vector2f(-1, -1));
	m_button.setActiveColor(cpp3ds::Color(200,255,255));
	m_button.setTextColor(cpp3ds::Color::Black);
	m_button.setTextActiveColor(cpp3ds::Color::Blue);
	m_button.setPosition(315 - m_button.getSize().x, 240);

	m_button.onClick([this]{
		ServerListItem* serverItem = m_serverList.getSelectedItem();
		if (serverItem) {
			m_serverList.deselect();

			if (m_menuVisible) {
				TweenEngine::Tween::to(m_button, gui3ds::Button::POSITION_Y, 0.4f)
						.target(240)
						.ease(TweenEngine::TweenEquations::easeInCubic)
						.setCallback(TweenEngine::TweenCallback::COMPLETE, [this](TweenEngine::BaseTween* source){
							m_menuVisible = false;
						})
						.start(m_tweenManager);
			}

			if (getContext().client.connect(serverItem->getIp(),
											serverItem->getPort()) == cpp3ds::Socket::Done)
			{
				requestStackPop();
				requestStackPush(States::Play);
				requestStackPush(States::NameSelect);
			} else {
				Notification::spawn(_("Failed to connect. Try again."));
			}
		}
	});
}

void ServerSelectState::renderTopScreen(cpp3ds::Window& window)
{
	window.draw(m_caption);
}

void ServerSelectState::renderBottomScreen(cpp3ds::Window& window)
{
	window.draw(m_serverList);
	window.draw(m_button);
}

bool ServerSelectState::update(float delta)
{
	m_serverList.update(delta);
	m_tweenManager.update(delta);
	return true;
}

bool ServerSelectState::processEvent(const cpp3ds::Event& event)
{
	m_button.processEvent(event);

	if (!m_serverList.processEvent(event))
	{
		if (m_serverList.getSelectedItem()) {
			if (!m_menuVisible) {
				TweenEngine::Tween::to(m_button, gui3ds::Button::POSITION_Y, 0.4f)
						.target(235 - m_button.getSize().y)
						.ease(TweenEngine::TweenEquations::easeOutCubic)
						.setCallback(TweenEngine::TweenCallback::COMPLETE, [this](TweenEngine::BaseTween* source){
							m_menuVisible = true;
						})
						.start(m_tweenManager);
			}
		}
	}

	if (event.type == cpp3ds::Event::KeyPressed && event.key.code == cpp3ds::Keyboard::B)
	{
		requestStackPop();
		requestStackPush(States::ServerSelect);
	}

	return true;
}

} // namespace DrawAttack
