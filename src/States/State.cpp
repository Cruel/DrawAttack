#include "State.hpp"
#include "StateStack.hpp"


namespace DrawAttack {

State::Context::Context(Client& client, cpp3ds::String& name, cpp3ds::String& data, TransitionData& transition, cpp3ds::Color& color)
: client(client)
, name(name)
, data(data)
, transition(transition)
, color(color)
{
	transition.nextStateID = States::None;
}

State::State(StateStack& stack, Context& context)
: m_stack(&stack)
, m_context(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	m_stack->pushState(stateID);
}

void State::requestStackPop(States::ID stateID)
{
	m_stack->popState();
}

void State::requestStackClear()
{
	m_stack->clearStates();
}

State::Context State::getContext() const
{
	return m_context;
}

} // namespace DrawAttack
