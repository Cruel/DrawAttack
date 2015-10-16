#ifndef DRAWATTACK_PLAYSTATE_HPP
#define DRAWATTACK_PLAYSTATE_HPP

#include "State.hpp"
#include "../DrawingBoard.hpp"
#include "../Keyboard/Keyboard.hpp"
#include <cpp3ds/Graphics/Sprite.hpp>
#include <cpp3ds/Graphics/Text.hpp>
#include <TweenEngine/TweenManager.h>

namespace DrawAttack {

class PlayState : public State
{
public:
	enum Mode {
		Spectate,
		Draw,
		Guess,
	};

	PlayState(StateStack& stack, Context& context);
	virtual void renderTopScreen(cpp3ds::Window& window);
	virtual void renderBottomScreen(cpp3ds::Window& window);
	virtual bool update(float delta);
	virtual bool processEvent(const cpp3ds::Event& event);

private:
	void changeMode(Mode mode);

	Mode         m_mode;
	DrawingBoard m_board;

	TweenEngine::TweenManager m_tweenManager;
	util3ds::Keyboard         m_keyboard;
};

} // namespace DrawAttack

#endif // DRAWATTACK_PLAYSTATE_HPP
