#include <TweenEngine/Tween.h>
#include "SpeechBubble.hpp"

namespace DrawAttack {

cpp3ds::Texture SpeechBubble::m_textureLeftBubble;
cpp3ds::Texture SpeechBubble::m_textureRightBubble;

SpeechBubble::SpeechBubble()
: m_visible(false)
, m_busy(false)
{
	if (m_textureLeftBubble.getSize().x == 0)
		m_textureLeftBubble.loadFromFile("images/speech-bubble-left.9.png");
	if (m_textureRightBubble.getSize().x == 0)
		m_textureRightBubble.loadFromFile("images/speech-bubble-right.9.png");

	setSide(Left);
	getText().setCharacterSize(12);
	setColor(cpp3ds::Color(150, 255, 255));
	setTextOffset(cpp3ds::Vector2f(-1, -1));

	TweenEngine::Tween::setWaypointsLimit(1);
}


SpeechBubble::~SpeechBubble()
{
	//
}


void SpeechBubble::setSide(SpeechBubble::Side side)
{
	setTexture(side == Left ? &m_textureLeftBubble : &m_textureRightBubble);
	m_side = side;
}


SpeechBubble::Side SpeechBubble::getSide() const
{
	return m_side;
}


bool SpeechBubble::isVisible() const
{
	return m_visible;
}


void SpeechBubble::popStringAnimate()
{
	if (m_busy || m_strings.empty())
		return;

	cpp3ds::String newString = m_strings.front();
	m_strings.pop();

	m_tweenManager.killAll();

	if (m_visible) {
		cpp3ds::String oldString = getString();
		setString(newString);
		autoSize();
		float newContentWidth = getContentSize().x;
		cpp3ds::Vector2f newPosition(m_side == Left ? m_position.x : m_position.x - getSize().x, m_position.y);
		setString(oldString);
		setContentSize(getContentSize()); // Reset content size and make autosize = false

		TweenEngine::Tween::to(*this, SpeechBubble::TEXTCOLOR_ALPHA, 0.15f)
			.target(0.f)
			.setCallback(TweenEngine::TweenCallback::COMPLETE, [this, newString](TweenEngine::BaseTween* source){
				setString(newString);
			})
			.start(m_tweenManager);
		TweenEngine::Tween::to(*this, SpeechBubble::CONTENT_X, 0.15f)
			.target(newContentWidth)
			.ease(TweenEngine::TweenEquations::easeOutCubic)
			.delay(0.15f)
			.start(m_tweenManager);
		TweenEngine::Tween::to(*this, SpeechBubble::POSITION_X, 0.15f)
			.target(newPosition.x)
			.ease(TweenEngine::TweenEquations::easeOutCubic)
			.delay(0.15f)
			.start(m_tweenManager);
	} else {
		setString(newString);
		autoSize();
		Transformable::setPosition(m_position.x, m_position.y + getSize().y);
		setScale(0.f, 0.f);
		setTextColor(cpp3ds::Color(0, 0, 0, 0));
		Button::setColor(m_color);

		TweenEngine::Tween::to(*this, SpeechBubble::POSITION_XY, 0.3f)
			.target(m_side == Left ? m_position.x : m_position.x - getSize().x, m_position.y)
			.ease(TweenEngine::TweenEquations::easeInCubic)
			.start(m_tweenManager);
		TweenEngine::Tween::to(*this, SpeechBubble::SCALE_XY, 0.3f)
			.target(1.f, 1.f)
			.ease(TweenEngine::TweenEquations::easeInCubic)
			.start(m_tweenManager);
	}

	// Color flicker
	TweenEngine::Tween::to(*this, SpeechBubble::COLOR_RGB, 1.f)
		.waypoint(m_color.r + 40, m_color.g + 40, m_color.b + 40)
		.target(m_color.r, m_color.g, m_color.b)
		.ease(TweenEngine::TweenEquations::easeOutCubic)
		.delay(0.4f)
		.start(m_tweenManager);
	TweenEngine::Tween::to(*this, SpeechBubble::COLOR_ALPHA, 1.f)
		.waypoint(m_color.a + 20.f)
		.target(m_color.a)
		.ease(TweenEngine::TweenEquations::easeOutCubic)
		.delay(0.4f)
		.start(m_tweenManager);

	TweenEngine::Tween::to(*this, SpeechBubble::COLOR_ALPHA, 0.4f)
		.target(static_cast<float>(m_color.a))
		.delay(m_visible ? 0.f : 0.3f)
		.start(m_tweenManager);
	TweenEngine::Tween::to(*this, SpeechBubble::TEXTCOLOR_ALPHA, 0.4f)
		.target(255.f)
		.delay(0.3f)
		.setCallback(TweenEngine::TweenCallback::COMPLETE, [this](TweenEngine::BaseTween* source){
			m_busy = false;
			popStringAnimate();
		})
		.start(m_tweenManager);

	TweenEngine::Tween::to(*this, SpeechBubble::COLOR_ALPHA, 3.f)
		.target(0.f)
		.delay(4.f)
		.start(m_tweenManager);
	TweenEngine::Tween::to(*this, SpeechBubble::TEXTCOLOR_ALPHA, 3.f)
		.target(0.f)
		.delay(4.f)
		.setCallback(TweenEngine::TweenCallback::COMPLETE, [this](TweenEngine::BaseTween* source){
			m_visible = false;
		})
		.start(m_tweenManager);

	m_visible = true;
	m_busy = true;
}


void SpeechBubble::update(float delta)
{
	m_tweenManager.update(delta);
}


void SpeechBubble::pushString(cpp3ds::String string)
{
	m_strings.push(string);
	popStringAnimate();
}


void SpeechBubble::setPosition(const cpp3ds::Vector2f &position)
{
	m_position = position;
}


void SpeechBubble::setPosition(float x, float y)
{
	setPosition(cpp3ds::Vector2f(x, y));
}


const cpp3ds::Vector2f &SpeechBubble::getPosition() const
{
	return m_position;
}


void SpeechBubble::setColor(const cpp3ds::Color &color)
{
	m_color = color;
}


const cpp3ds::Color &SpeechBubble::getColor() const
{
	return m_color;
}

} // namesapce DrawAttack
