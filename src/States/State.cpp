#include "State.hpp"
#include "StateStack.hpp"


namespace DrawAttack {

State::Context::Context(Client& client, cpp3ds::String& name, cpp3ds::String& data)
: client(client)
, name(name)
, data(data)
{
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

void State::requestStackPop()
{
	m_stack->popState();
}

void State::requestStateClear()
{
	m_stack->clearStates();
}

State::Context State::getContext() const
{
	return m_context;
}

} // namespace DrawAttack
