#include "ServerSelectState.hpp"
#include "../Notification.hpp"
#include <TweenEngine/Tween.h>

namespace DrawAttack {

ServerSelectState::ServerSelectState(StateStack& stack, Context& context)
: State(stack, context)
, m_menuVisible(false)
{
	m_caption.setString(_("Select a server:"));
	m_caption.setFillColor(cpp3ds::Color::Black);
	m_caption.setPosition(10.f, 100.f);

	m_buttonTexture.loadFromFile("images/button.9.png");
	m_iconFont.loadFromFile("fonts/fontawesome.ttf");

	m_buttonRefresh.setTexture(&m_buttonTexture);
	m_buttonRefresh.setString(L"\uf021");
	m_buttonRefresh.getText().setCharacterSize(20);
	m_buttonRefresh.getText().setFont(m_iconFont);
	m_buttonRefresh.setColor(cpp3ds::Color(255,255,255,200));
	m_buttonRefresh.setActiveColor(cpp3ds::Color(200,255,255));
	m_buttonRefresh.setTextColor(cpp3ds::Color::Black);
	m_buttonRefresh.setTextActiveColor(cpp3ds::Color::Blue);
	m_buttonRefresh.setPosition(0, 240.f - m_buttonRefresh.getSize().y);

	m_buttonRefresh.onClick([this]{
		requestStackClear();
		requestStackPush(States::ServerSelect);
	});

	m_buttonConnect.setTexture(&m_buttonTexture);
	m_buttonConnect.setString(_("Connect"));
	m_buttonConnect.getText().setCharacterSize(18);
	m_buttonConnect.setTextOffset(cpp3ds::Vector2f(-1, -1));
	m_buttonConnect.setActiveColor(cpp3ds::Color(200,255,255));
	m_buttonConnect.setTextColor(cpp3ds::Color::Black);
	m_buttonConnect.setTextActiveColor(cpp3ds::Color::Blue);
	m_buttonConnect.setPosition(315 - m_buttonConnect.getSize().x, 240);

	m_buttonConnect.onClick([this]{
		ServerListItem* serverItem = m_serverList.getSelectedItem();
		if (serverItem) {
			m_serverList.deselect();

			if (m_menuVisible) {
				TweenEngine::Tween::to(m_buttonConnect, gui3ds::Button::POSITION_Y, 0.4f)
					.target(240)
					.ease(TweenEngine::TweenEquations::easeInCubic)
					.setCallback(TweenEngine::TweenCallback::COMPLETE, [this](TweenEngine::BaseTween* source){
						m_menuVisible = false;
					})
					.start(m_tweenManager);
			}

			if (getContext().client.connect(serverItem->getIp(), serverItem->getPort()) == cpp3ds::Socket::Done)
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
	window.draw(m_buttonConnect);
	if (!m_serverList.isLoading())
		window.draw(m_buttonRefresh);
}

bool ServerSelectState::update(float delta)
{
	m_serverList.update(delta);
	m_tweenManager.update(delta);
	return true;
}

bool ServerSelectState::processEvent(const cpp3ds::Event& event)
{
	m_buttonConnect.processEvent(event);
	if (!m_serverList.isLoading())
		m_buttonRefresh.processEvent(event);

	if (!m_serverList.processEvent(event))
	{
		if (m_serverList.getSelectedItem()) {
			if (!m_menuVisible) {
				TweenEngine::Tween::to(m_buttonConnect, gui3ds::Button::POSITION_Y, 0.4f)
					.target(235 - m_buttonConnect.getSize().y)
					.ease(TweenEngine::TweenEquations::easeOutCubic)
					.setCallback(TweenEngine::TweenCallback::COMPLETE, [this](TweenEngine::BaseTween* source){
						m_menuVisible = true;
					})
					.start(m_tweenManager);
			}
		}
	}

	if (event.type == cpp3ds::Event::KeyPressed)
	{
		if (event.key.code == cpp3ds::Keyboard::B || event.key.code == cpp3ds::Keyboard::Select) {
			requestStackPop();
			requestStackPush(States::Title);
		}
	}

	return true;
}

} // namespace DrawAttack
