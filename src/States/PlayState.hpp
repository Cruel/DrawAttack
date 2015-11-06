#ifndef DRAWATTACK_PLAYSTATE_HPP
#define DRAWATTACK_PLAYSTATE_HPP

#include "State.hpp"
#include "../DrawingBoard.hpp"
#include "../Keyboard/Keyboard.hpp"
#include "../GUI/Button.hpp"
#include "../SpeechBubble.hpp"
#include "../Network/Player.hpp"
#include "../ScoreBoard.hpp"
#include <cpp3ds/Graphics/Sprite.hpp>
#include <cpp3ds/Graphics/Text.hpp>
#include <TweenEngine/TweenManager.h>

namespace DrawAttack {

class PlayState : public State
{
public:
	enum Mode {
		Spectate,
		Wait,
		Draw,
		Guess,
	};

	PlayState(StateStack& stack, Context& context);
	~PlayState();
	virtual void renderTopScreen(cpp3ds::Window& window);
	virtual void renderBottomScreen(cpp3ds::Window& window);
	virtual bool update(float delta);
	virtual bool processEvent(const cpp3ds::Event& event);

private:
	struct PlayerData {
		PlayerData(std::string name) : player(name) {}
		Player player;
		SpeechBubble bubble;
	};

	void changeMode(Mode mode);
	bool processNetworkEvent(const NetworkEvent& event);

	void updatePlayerInfo();
	void setTimerPosition(bool top);

	cpp3ds::Texture m_textBackgroundTexture;

	gui3ds::Button m_countdownText;
	cpp3ds::Clock m_countdownClock;
	float m_roundDuration;
	std::string m_roundWord;
	std::string m_roundDrawer;
	gui3ds::Button m_roundWordText;

	Mode          m_mode;
	std::queue<Mode> m_modeChangeQueue;
	DrawingBoard  m_board;
	DrawingBoard *m_boardCopy;

	TweenEngine::TweenManager m_tweenManager;
	util3ds::Keyboard         m_keyboard;

	std::map<std::string, PlayerData> m_players;

	cpp3ds::Font    m_iconFont;
	cpp3ds::Texture m_buttonTexture;
	gui3ds::Button  m_buttonUndo;
	gui3ds::Button  m_buttonClear;

	ScoreBoard m_scoreBoard;
};

} // namespace DrawAttack

#endif // DRAWATTACK_PLAYSTATE_HPP
