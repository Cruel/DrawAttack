#ifndef DRAWATTACK_SERVERSELECTSTATE_HPP
#define DRAWATTACK_SERVERSELECTSTATE_HPP

#include "State.hpp"
#include "../ServerList.hpp"
#include "../GUI/Button.hpp"
#include "../GUI/NinePatch.hpp"
#include "../SpeechBubble.hpp"
#include "../ScoreBoard.hpp"

namespace DrawAttack {

class ServerSelectState : public State
{
public:
	ServerSelectState(StateStack& stack, Context& context);

	virtual void renderTopScreen(cpp3ds::Window& window);
	virtual void renderBottomScreen(cpp3ds::Window& window);
	virtual bool update(float delta);
	virtual bool processEvent(const cpp3ds::Event& event);

private:
	cpp3ds::Font m_iconFont;
	cpp3ds::Text m_caption;
	ServerList m_serverList;
	cpp3ds::Texture m_buttonTexture;
	gui3ds::Button m_button;
	TweenEngine::TweenManager m_tweenManager;
	bool m_menuVisible;
};

} // namespace DrawAttack

#endif // DRAWATTACK_SERVERSELECTSTATE_HPP
