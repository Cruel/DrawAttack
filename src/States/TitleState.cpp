#include "TitleState.hpp"
#include <cpp3ds/Window/Window.hpp>


namespace DrawAttack {

TitleState::TitleState(StateStack& stack, Context& context)
: State(stack, context)
, mText()
, mShowText(true)
, mTextEffectTime(0)
{
	mText.setString("Press any key to start");
}

void TitleState::renderTopScreen(cpp3ds::Window& window)
{
	if (mShowText)
		window.draw(mText);
}

void TitleState::renderBottomScreen(cpp3ds::Window& window)
{
	// Nothing
}

bool TitleState::update(float delta)
{
	mTextEffectTime += delta;

	if (mTextEffectTime >= 0.5f)
	{
		mShowText = !mShowText;
		mTextEffectTime = 0;
	}

	return true;
}

bool TitleState::processEvent(const cpp3ds::Event& event)
{
	// If any key is pressed, trigger the next screen
	if (event.type == cpp3ds::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}

	return true;
}

} // namespace DrawAttack
