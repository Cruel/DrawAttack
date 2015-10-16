#ifndef DRAWATTACK_DISCONNECTEDSTATE_HPP
#define DRAWATTACK_DISCONNECTEDSTATE_HPP

#include "State.hpp"
#include <cpp3ds/Graphics/Sprite.hpp>
#include <cpp3ds/Graphics/Text.hpp>

namespace DrawAttack {

class DisconnectedState : public State
{
public:
	DisconnectedState(StateStack& stack, Context& context);

	virtual void renderTopScreen(cpp3ds::Window& window);
	virtual void renderBottomScreen(cpp3ds::Window& window);
	virtual bool update(float delta);
	virtual bool processEvent(const cpp3ds::Event& event);

private:
	cpp3ds::Text m_text;
};

} // namespace DrawAttack

#endif // DRAWATTACK_DISCONNECTEDSTATE_HPP
