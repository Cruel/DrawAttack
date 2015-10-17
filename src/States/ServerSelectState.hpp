#ifndef DRAWATTACK_SERVERSELECTSTATE_HPP
#define DRAWATTACK_SERVERSELECTSTATE_HPP

#include "State.hpp"
#include "../ServerList.hpp"
#include "../GUI/Button.hpp"

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
	ServerList m_serverList;
	util3ds::Button m_button;
};

} // namespace DrawAttack

#endif // DRAWATTACK_SERVERSELECTSTATE_HPP
