#ifndef DRAWATTACK_PAUSESTATE_HPP
#define DRAWATTACK_PAUSESTATE_HPP

#include "State.hpp"
#include <cpp3ds/Graphics/Sprite.hpp>
#include <cpp3ds/Graphics/Text.hpp>

namespace DrawAttack {

	class PauseState : public State
	{
	public:
		PauseState(StateStack& stack, Context& context);

		virtual void renderTopScreen(cpp3ds::Window& window);
		virtual void renderBottomScreen(cpp3ds::Window& window);
		virtual bool update(float delta);
		virtual bool processEvent(const cpp3ds::Event& event);

	private:
		cpp3ds::Text m_text;
	};

} // namespace DrawAttack

#endif // DRAWATTACK_PAUSESTATE_HPP
