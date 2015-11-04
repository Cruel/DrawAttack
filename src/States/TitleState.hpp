#ifndef DRAWATTACK_TITLESTATE_HPP
#define DRAWATTACK_TITLESTATE_HPP

#include "State.hpp"
#include <cpp3ds/Graphics/Sprite.hpp>
#include <cpp3ds/Graphics/Text.hpp>

namespace DrawAttack {

class TitleState : public State
{
public:
	TitleState(StateStack& stack, Context& context);

	virtual void renderTopScreen(cpp3ds::Window& window);
	virtual void renderBottomScreen(cpp3ds::Window& window);
	virtual bool update(float delta);
	virtual bool processEvent(const cpp3ds::Event& event);


private:
	cpp3ds::Text m_textTitle;
	cpp3ds::Text m_text;

	bool m_showText;
	float m_textEffectTime;
};

} // namespace DrawAttack

#endif // DRAWATTACK_TITLESTATE_HPP
