#ifndef DRAWATTACK_NAMESELECTSTATE_HPP
#define DRAWATTACK_NAMESELECTSTATE_HPP

#include "State.hpp"
#include "../Keyboard/Keyboard.hpp"
#include <cpp3ds/Graphics/Sprite.hpp>
#include <cpp3ds/Graphics/Text.hpp>

namespace DrawAttack {

class NameSelectState : public State
{
public:
	NameSelectState(StateStack& stack, Context& context);

	virtual void renderTopScreen(cpp3ds::Window& window);
	virtual void renderBottomScreen(cpp3ds::Window& window);
	virtual bool update(float delta);
	virtual bool processEvent(const cpp3ds::Event& event);

private:
	cpp3ds::Text m_text;
	cpp3ds::RectangleShape m_rectangle;
	util3ds::Keyboard m_keyboard;
};

} // namespace DrawAttack

#endif // DRAWATTACK_NAMESELECTSTATE_HPP
