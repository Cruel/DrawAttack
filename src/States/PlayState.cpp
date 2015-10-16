#include "PlayState.hpp"
#include <cpp3ds/Window/Window.hpp>
#include <TweenEngine/Tween.h>

using namespace TweenEngine;
using namespace cpp3ds;

namespace DrawAttack {

PlayState::PlayState(StateStack& stack, Context& context)
: State(stack, context)
, m_mode(Mode::Spectate)
{
	m_board.create(320, 240);
	m_board.setLineThickness(1);

	m_keyboard.loadFromFile("kb/keyboard.xml");

	changeMode(Mode::Spectate);
}

void PlayState::renderTopScreen(cpp3ds::Window& window)
{
	if (m_mode != Mode::Draw) {
		window.draw(m_board);
	}
}

void PlayState::renderBottomScreen(cpp3ds::Window& window)
{
	if (m_mode == Mode::Draw) {
		window.draw(m_board);
	}

	if (m_mode == Mode::Guess) {
		window.draw(m_keyboard);
	}
}

bool PlayState::update(float delta)
{
	NetworkEvent event;
	while (getContext().client.pollEvent(event)) {
		switch (event.type) {
			case NetworkEvent::PlayerConnected:
			std::cout << "your name: " << getContext().name.toAnsiString() << std::endl;
				if (event.player.name.compare(getContext().name.toAnsiString()) == 0) {
					if (m_mode == Mode::Spectate) {
						changeMode(Mode::Guess);
						requestStackPop();
					}
					std::cout << "You joined with name: " << event.player.name << std::endl;
				} else {
					std::cout << event.player.name << " joined!" << std::endl;
				}
				break;
			case NetworkEvent::ServerShutdown:
				requestStackPop();
				requestStackPush(States::Disconnected);
				goto exit_while;
			case NetworkEvent::PlayerData:
				std::cout << (int)event.playerData.count << " players connected:" << std::endl;
				break;
			case NetworkEvent::Text:
				std::cout << event.text.value.toAnsiString() << std::endl;
				break;
			case NetworkEvent::DrawerDesignation:
				if (event.player.name.compare(getContext().name.toAnsiString()) == 0) {
					changeMode(Mode::Draw);
				} else {
					changeMode(Mode::Guess);
				}
				std::cout << event.player.name << " is drawing now" << std::endl;
				break;
			case NetworkEvent::DrawMove:
				m_board.addPoint(event.draw.x, event.draw.y);
				break;
			case NetworkEvent::DrawEndline:
				m_board.endLine();
				break;
			default:
				break;
		}
	}
exit_while:

	if (m_mode == Mode::Guess) {
		String text;
		if (m_keyboard.popString(text)) {
			getContext().client.sendText(text);
		}
	}
	m_keyboard.update(delta);

	m_tweenManager.update(delta);
	return true;
}

bool PlayState::processEvent(const cpp3ds::Event& event)
{
	if (m_mode == Mode::Guess)
	{
		if (!m_keyboard.processEvents(event))
			return false;
	}

	if (m_mode == Mode::Draw)
	{
		if (event.type == Event::TouchMoved) {
			const Vector2f p = m_board.getInverseTransform().transformPoint(event.touch.x, event.touch.y);
			getContext().client.sendDrawMove(p.x, p.y);
		}
		if (event.type == Event::TouchEnded) {
			const Vector2f p = m_board.getInverseTransform().transformPoint(event.touch.x, event.touch.y);
			getContext().client.sendDrawEndline(p.x, p.y);
		}
		if (event.type == Event::KeyReleased) {
			if (event.key.code == Keyboard::L) {
				std::cout << "Starting tween in 1 second...";
				m_board.setPosition(30, 30);
				Tween::to(m_board, DrawingBoard::POSITION_XY, 0.5f)
						.target(300, 200)
						.ease(TweenEquations::easeOutBounce)
//					.delay(1.0f)
						.repeatYoyo(2, 0.2f)
						.start(m_tweenManager);
			}
			if (event.key.code == Keyboard::A) {
				std::cout << "Rotating drawing board...";
				Tween::to(m_board, DrawingBoard::ROTATION, 0.5f)
						.target(90)
						.ease(TweenEquations::easeOutBounce)
//					.delay(1.0f)
//					.repeatYoyo(2, 0.2f)
						.start(m_tweenManager);
			}
		}
	}

	return true;
}


void PlayState::changeMode(PlayState::Mode mode)
{
	if (mode == Mode::Draw) {
		m_board.setPosition(0, 0);
	} else {
		m_board.setPosition(40, 0);
	}
	m_mode = mode;
}


} // namespace DrawAttack
