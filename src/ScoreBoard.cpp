#include <TweenEngine/Tween.h>
#include "ScoreBoard.hpp"

namespace DrawAttack {


ScoreBoard::ScoreBoard()
: m_visible(false)
{
	m_overlay.setContentSize(cpp3ds::Vector2f(400.f, 240.f));
	m_overlay.setColor(cpp3ds::Color::Transparent);

	m_texture.loadFromFile("images/button-radius.9.png");
	setTexture(&m_texture);
	setContentSize(cpp3ds::Vector2f(300.f, 200.f));
	setPadding(0.f, 0.f, 16.f, 16.f);
	setPosition(50.f, 20.f);
	setColor(cpp3ds::Color::Transparent);
}


void ScoreBoard::draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const
{
	target.draw(m_overlay);
	Button::draw(target, states);

	states.transform *= getTransform();
	for (auto& text : m_texts) {
		target.draw(text, states);
	}
}


bool ScoreBoard::isVisible() const
{
	return m_visible;
}


void ScoreBoard::show()
{
	m_visible = true;
	m_tweenManager.killAll();
	TweenEngine::Tween::to(m_overlay, COLOR_ALPHA, 0.3f).target(100.f).start(m_tweenManager);
	TweenEngine::Tween::to(*this, COLOR_ALPHA, 0.3f)
		.target(220.f)
		.start(m_tweenManager);
}


void ScoreBoard::hide()
{
	m_tweenManager.killAll();
	TweenEngine::Tween::to(m_overlay, COLOR_ALPHA, 0.3f).target(0.f).start(m_tweenManager);
	TweenEngine::Tween::to(*this, COLOR_ALPHA, 0.3f)
		.target(0.f)
		.setCallback(TweenEngine::TweenCallback::COMPLETE, [this](TweenEngine::BaseTween* source){
			m_visible = false;
		})
		.start(m_tweenManager);
}


void ScoreBoard::update(float delta)
{
	m_tweenManager.update(delta);
}


void ScoreBoard::addText(cpp3ds::String string)
{
	cpp3ds::Text text;
	text.setString(string);
	text.setCharacterSize(12);
	text.setPosition(15.f, 15.f + 20.f * m_texts.size());
	m_texts.emplace_back(text);
}


void ScoreBoard::clear()
{
	m_texts.clear();
}

} // namespace DrawAttack
