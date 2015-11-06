#include "PlayState.hpp"
#include "../Notification.hpp"
#include <cpp3ds/Window/Window.hpp>
#include <TweenEngine/Tween.h>


namespace DrawAttack {

PlayState::PlayState(StateStack& stack, Context& context)
: State(stack, context)
, m_mode(Mode::Spectate)
, m_boardCopy(nullptr)
{
	m_textBackgroundTexture.loadFromFile("images/button-radius.9.png");
	m_buttonTexture.loadFromFile("images/button.9.png");
	m_iconFont.loadFromFile("fonts/fontawesome.ttf");

	m_buttonClear.setTexture(&m_buttonTexture);
	m_buttonClear.getText().setCharacterSize(24);
	m_buttonClear.setTextOffset(cpp3ds::Vector2f(-1, -1));
	m_buttonClear.setColor(cpp3ds::Color(200,200,255));
	m_buttonClear.setActiveColor(cpp3ds::Color(150,150,255));
	m_buttonClear.getText().setFont(m_iconFont);
	m_buttonClear.setString(L"\uf1f8");
	m_buttonClear.setPosition(0, 240 - m_buttonClear.getSize().y);
	m_buttonClear.onClick([this]{
		getContext().client.sendClear();
		m_board.clear();
	});

	m_buttonUndo.setTexture(&m_buttonTexture);
	m_buttonUndo.getText().setCharacterSize(24);
	m_buttonUndo.setColor(cpp3ds::Color(200,200,255));
	m_buttonUndo.setActiveColor(cpp3ds::Color(150,150,255));
	m_buttonUndo.getText().setFont(m_iconFont);
	m_buttonUndo.setString(L"\uf0e2");
	m_buttonUndo.setPosition(m_buttonClear.getSize().x, 240 - m_buttonUndo.getSize().y);
	m_buttonUndo.onClick([this]{
		getContext().client.sendUndo();
		m_board.undo();
	});

	m_board.create(320, 240);
	m_board.setLineThickness(1);

	m_roundWordText.setTexture(&m_textBackgroundTexture);
	m_roundWordText.setColor(cpp3ds::Color(255, 255, 255, 50));
	m_roundWordText.getText().setCharacterSize(30);
	m_roundWordText.getText().setStyle(cpp3ds::Text::Bold);

	m_countdownText.setTexture(&m_textBackgroundTexture);

	m_keyboard.loadFromFile("kb/keyboard.xml");

	changeMode(Mode::Spectate);
}


void PlayState::renderTopScreen(cpp3ds::Window& window)
{
	if (m_mode == Mode::Draw) {
		if (m_boardCopy) {
			window.draw(m_board);
		} else {
			window.draw(m_countdownText);
		}

		if (!m_roundWord.empty())
			window.draw(m_roundWordText);
	} else {
		if (m_boardCopy) {
			window.draw(*m_boardCopy);
		} else {
			window.draw(m_board);
		}
	}

	for (auto& player : m_players) {
		if (player.second.bubble.isVisible())
			window.draw(player.second.bubble);
	}

	// Draw scoreboard on top
	if (m_scoreBoard.isVisible())
		window.draw(m_scoreBoard);
}


void PlayState::renderBottomScreen(cpp3ds::Window& window)
{
	if (m_mode == Mode::Draw) {
		if (m_boardCopy) {
			window.draw(*m_boardCopy);
			window.draw(m_keyboard);
		} else {
			window.draw(m_board);
		}
		window.draw(m_buttonUndo);
		window.draw(m_buttonClear);
	} else {
		if (m_boardCopy) {
			window.draw(m_board);
			window.draw(m_buttonUndo);
			window.draw(m_buttonClear);
		}
	}

	if (m_mode == Mode::Guess) {
		window.draw(m_keyboard);
		if (!m_boardCopy)
			window.draw(m_countdownText);
	}

	if (m_mode == Mode::Wait) {
		window.draw(m_keyboard);
	}
}


bool PlayState::update(float delta)
{
	NetworkEvent event;
	cpp3ds::Packet packet;

	// Update network events
	if (getContext().client.pollPacket(packet)) {
		while (NetworkEvent::packetToEvent(packet, event))
			if (!processNetworkEvent(event))
				break;
	} else {
		event.type = NetworkEvent::ServerShutdown;
		event.server.message = "(None)";
		processNetworkEvent(event);
	}
	getContext().client.flushPacket();

	// Update keyboard
	if (m_mode != Mode::Draw) {
		cpp3ds::String text;
		if (m_keyboard.popString(text)) {
			getContext().client.sendText(getContext().name, text);
		}
	}
	m_keyboard.update(delta);

	// Update text bubbles
	for (auto& player : m_players) {
		player.second.bubble.update(delta);
	}

	// Update countdown timer(s)
	if (m_mode != Mode::Wait) {
		float time = m_roundDuration - m_countdownClock.getElapsedTime().asSeconds();
		if (time < 0.f)
			time = 0.f;
		m_countdownText.setString(_("%.0f", time));
	}

	m_scoreBoard.update(delta);

	m_tweenManager.update(delta);
	return true;
}


bool PlayState::processEvent(const cpp3ds::Event& event)
{
	if (m_mode == Mode::Guess || m_mode == Mode::Wait)
	{
		if (!m_keyboard.processEvents(event))
			return false;
	}

	if (m_mode == Mode::Draw)
	{
		if (!m_buttonClear.processEvent(event))
			return false;
		if (!m_buttonUndo.processEvent(event))
			return false;

		if (event.type == cpp3ds::Event::TouchMoved) {
			if (!m_boardCopy) {
				const cpp3ds::Vector2f p = m_board.getInverseTransform().transformPoint(event.touch.x, event.touch.y);
				m_board.addPoint(p.x, p.y);
				getContext().client.sendDrawMove(p.x, p.y);
			}
		}
		if (event.type == cpp3ds::Event::TouchEnded) {
			if (!m_boardCopy) {
				const cpp3ds::Vector2f p = m_board.getInverseTransform().transformPoint(event.touch.x, event.touch.y);
				m_board.endLine();
				getContext().client.sendDrawEndline(p.x, p.y);
			}
		}
	}

	if (event.type == cpp3ds::Event::KeyPressed)
	{
		if (event.key.code == cpp3ds::Keyboard::Y) {
			m_scoreBoard.show();
		}
		if (event.key.code == cpp3ds::Keyboard::Start) {
			requestStackPush(States::Pause);
		}
		return false;
	}

	if (event.type == cpp3ds::Event::KeyReleased)
	{
		if (event.key.code == cpp3ds::Keyboard::Y) {
			m_scoreBoard.hide();
		}
	}

	return true;
}


void PlayState::changeMode(PlayState::Mode mode)
{
	if (m_mode == mode)
		return;
	if (m_boardCopy) {
		m_modeChangeQueue.push(mode);
		return;
	}

	auto tweenCallback = [this, mode](TweenEngine::BaseTween* source) {
		m_mode = mode;
		delete m_boardCopy;
		m_boardCopy = nullptr;
		if (!m_modeChangeQueue.empty()) {
			changeMode(m_modeChangeQueue.front());
			m_modeChangeQueue.pop();
		}
	};

	if (m_mode == Mode::Draw) {
		m_boardCopy = new DrawingBoard(m_board);
		m_board.setPosition(40, 290);

		setTimerPosition(false);

		TweenEngine::Tween::to(*m_boardCopy, DrawingBoard::POSITION_Y, 1.f)
			.target(-290.f)
			.ease(TweenEngine::TweenEquations::easeOutCubic)
			.setCallback(TweenEngine::TweenCallback::COMPLETE, tweenCallback)
			.start(m_tweenManager);
		TweenEngine::Tween::to(m_board, DrawingBoard::POSITION_Y, 1.f)
			.target(0.f)
			.ease(TweenEngine::TweenEquations::easeOutCubic)
			.start(m_tweenManager);

		m_keyboard.setPosition(0.f, 240.f);
		TweenEngine::Tween::to(m_keyboard, DrawingBoard::POSITION_Y, 1.f)
			.target(0.f)
			.ease(TweenEngine::TweenEquations::easeOutCubic)
			.start(m_tweenManager);
		TweenEngine::Tween::to(m_buttonUndo, gui3ds::Button::COLOR_ALPHA, 0.5f).target(0.f).start(m_tweenManager);
		TweenEngine::Tween::to(m_buttonUndo, gui3ds::Button::TEXTCOLOR_ALPHA, 0.5f).target(0.f).start(m_tweenManager);
		TweenEngine::Tween::to(m_buttonClear, gui3ds::Button::COLOR_ALPHA, 0.5f).target(0.f).start(m_tweenManager);
		TweenEngine::Tween::to(m_buttonClear, gui3ds::Button::TEXTCOLOR_ALPHA, 0.5f).target(0.f).start(m_tweenManager);
	}
	else
	{
		TweenEngine::Tween::to(m_buttonUndo, gui3ds::Button::COLOR_ALPHA, 2.f).target(230.f).delay(1.f).start(m_tweenManager);
		TweenEngine::Tween::to(m_buttonUndo, gui3ds::Button::TEXTCOLOR_ALPHA, 2.f).target(255.f).delay(1.f).start(m_tweenManager);
		TweenEngine::Tween::to(m_buttonClear, gui3ds::Button::COLOR_ALPHA, 2.f).target(230.f).delay(1.f).start(m_tweenManager);
		TweenEngine::Tween::to(m_buttonClear, gui3ds::Button::TEXTCOLOR_ALPHA, 2.f).target(255.f).delay(1.f).start(m_tweenManager);

		if (mode == Mode::Draw) {
			setTimerPosition(true);
			m_boardCopy = new DrawingBoard(m_board);
			TweenEngine::Tween::to(*m_boardCopy, DrawingBoard::POSITION_Y, 1.f)
				.target(-240.f)
				.ease(TweenEngine::TweenEquations::easeOutCubic)
				.setCallback(TweenEngine::TweenCallback::COMPLETE, tweenCallback)
				.start(m_tweenManager);
			m_board.setPosition(0.f, -240.f);
			TweenEngine::Tween::to(m_board, DrawingBoard::POSITION_Y, 1.f)
				.target(0.f)
				.ease(TweenEngine::TweenEquations::easeOutCubic)
				.start(m_tweenManager);

			TweenEngine::Tween::to(m_keyboard, DrawingBoard::POSITION_Y, 1.f)
				.target(240.f)
				.ease(TweenEngine::TweenEquations::easeOutCubic)
				.start(m_tweenManager);
		} else {
			setTimerPosition(false);
			m_mode = mode;
		}
	}

	if (mode == Mode::Spectate)
		m_mode = mode;
}


bool PlayState::processNetworkEvent(const NetworkEvent &event)
{
	switch (event.type) {
		case NetworkEvent::PlayerConnected: {
			if (event.player.name.compare(getContext().name.toAnsiString()) == 0) {
				if (m_mode == Mode::Spectate) {
					changeMode(Mode::Wait);
					requestStackPop();
				}
			}
			Notification::spawn(_("%s joined the game.", event.player.name.c_str()));
			PlayerData data(event.player.name);
			data.bubble.setPosition(0, 40 * (m_players.size() + 1));
			m_players.emplace(event.player.name, data);
			updatePlayerInfo();
			break;
		}
		case NetworkEvent::PlayerDisconnected: {
			Notification::spawn(_("%s left the game.", event.player.name.c_str()));
			m_players.erase(event.player.name);
			updatePlayerInfo();
			break;
		}
		case NetworkEvent::ServerShutdown:
			getContext().transition.message = "Disconnected from server.";
			getContext().transition.nextStateID = States::Title;
			requestStackClear();
			requestStackPush(States::TransitionMessage);
			return false;
		case NetworkEvent::PlayerData: {
			int i = 1;
			std::cout << event.playerData.size() << " players connected:" << std::endl;
			for (const auto &player : event.playerData) {
				std::cout << player.name << std::endl;
				PlayerData data(player.name);
				data.player.setScore(player.score);
				m_players.emplace(player.name, data);
			}
			updatePlayerInfo();
			break;
		}
		case NetworkEvent::Text: {
			auto player = m_players.find(event.text.name);
			if (player != m_players.end()) {
				player->second.bubble.pushString(event.text.value);
				std::cout << event.text.name << ": " << event.text.value.toAnsiString() << std::endl;
			}
			break;
		}
		case NetworkEvent::RoundStart:
			if (m_mode != Mode::Spectate) {
				if (event.round.player.compare(getContext().name.toAnsiString()) == 0) {
					changeMode(Mode::Draw);
				} else {
					changeMode(Mode::Guess);
				}
			}
			Notification::spawn(_("%s is now drawing.", event.round.player.c_str()));
			m_roundDuration = event.round.time;
			m_roundDrawer = event.round.player;
			m_countdownClock.restart();
			m_roundWord.clear();
			m_board.clear();
			break;
		case NetworkEvent::RoundWord:
			m_roundWord = event.roundWord;
			m_roundWordText.setString(m_roundWord);
			m_roundWordText.setPosition(std::round(200.f - m_roundWordText.getSize().x / 2),
			                            std::round(235.f - m_roundWordText.getSize().y));
			break;
		case NetworkEvent::RoundWin: {
			Notification::spawn(_("%s guessed the word: %s", event.round.player.c_str(), m_roundWord.c_str()));
			auto player = m_players.find(event.round.player);
			if (player != m_players.end()) {
				player->second.player.incrementScore();
				updatePlayerInfo();
			}
			if (m_mode != Mode::Spectate) {
				changeMode(Mode::Wait);
			}
			break;
		}
		case NetworkEvent::RoundFail:
			Notification::spawn(_("Round ended."));
			Notification::spawn(_("Word was: %s", m_roundWord.c_str()));
			if (m_mode != Mode::Spectate) {
				changeMode(Mode::Wait);
			}
			break;
		case NetworkEvent::RoundPass:
			Notification::spawn(_("%s passed.", m_roundDrawer));
			Notification::spawn(_("Word was: %s", m_roundWord.c_str()));
			if (m_mode != Mode::Spectate)
				changeMode(Mode::Wait);
			break;
		case NetworkEvent::WaitForPlayers:
			if (m_mode == Mode::Spectate)
				break;
			changeMode(Mode::Wait);
			Notification::spawn(_("Game suspended until there are %d players.", static_cast<int>(event.wait.value)));
			break;
		case NetworkEvent::DrawUndo:
			if (m_mode != Draw)
				m_board.undo();
			break;
		case NetworkEvent::DrawClear:
			if (m_mode != Draw)
				m_board.clear();
			break;
		case NetworkEvent::DrawMove:
			if (m_mode != Draw)
				m_board.addPoint(event.draw.x, event.draw.y);
			break;
		case NetworkEvent::DrawEndline:
			if (m_mode != Draw)
				m_board.endLine();
			break;
		case NetworkEvent::Ping:
			// Respond to PING so we're not disconnected
			getContext().client.sendPing();
			break;
		default:
			break;
	}
	return true;
}


void PlayState::updatePlayerInfo()
{
	// Update player score board
	m_scoreBoard.clear();
	for (auto& player : m_players) {
		m_scoreBoard.addText(_("%d\t%s", player.second.player.getScore(), player.second.player.getName().c_str()));
	}

	// Update speech bubble positioning
	int i = 0;
	int count = m_players.size();
	if (count == 0)
		return;
	float y = 230 / (count%2 == 0 ? count : count+1);
	for (auto& player : m_players) {
		if (i % 2 == 0) {
			player.second.bubble.setSide(SpeechBubble::Left);
			player.second.bubble.setPosition(0, y * (i+1) - 10);
		} else {
			player.second.bubble.setSide(SpeechBubble::Right);
			player.second.bubble.setPosition(400, y * i - 10);
		}
		i++;
	}
}


void PlayState::setTimerPosition(bool top)
{
	if (top) {
		m_countdownText.getText().setColor(cpp3ds::Color(100, 0, 0));
		m_countdownText.getText().setCharacterSize(22);
		m_countdownText.setColor(cpp3ds::Color(255, 255, 255, 50));
		m_countdownText.setPosition(cpp3ds::Vector2f(180, 100));
	} else {
		m_countdownText.getText().setColor(cpp3ds::Color(100, 0, 0));
		m_countdownText.getText().setCharacterSize(16);
		m_countdownText.setColor(cpp3ds::Color(0, 0, 0, 50));
		m_countdownText.setPosition(cpp3ds::Vector2f(290, 5));
	}
}

} // namespace DrawAttack
