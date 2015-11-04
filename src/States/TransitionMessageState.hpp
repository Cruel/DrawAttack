#ifndef DRAWATTACK_TRANSITIONMESSAGESTATE_HPP
#define DRAWATTACK_TRANSITIONMESSAGESTATE_HPP

#include "State.hpp"
#include <cpp3ds/Graphics/Sprite.hpp>
#include <cpp3ds/Graphics/Text.hpp>
#include <cpp3ds/Graphics/RectangleShape.hpp>

namespace DrawAttack {

class TransitionMessageState : public State
{
public:
	TransitionMessageState(StateStack& stack, Context& context);

	virtual void renderTopScreen(cpp3ds::Window& window);
	virtual void renderBottomScreen(cpp3ds::Window& window);
	virtual bool update(float delta);
	virtual bool processEvent(const cpp3ds::Event& event);

private:
	cpp3ds::Text m_text;
	cpp3ds::RectangleShape m_rectangle;
};

} // namespace DrawAttack

#endif // DRAWATTACK_TRANSITIONMESSAGESTATE_HPP
