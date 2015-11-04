#ifndef DRAWATTACK_SPEECHBUBBLE_HPP
#define DRAWATTACK_SPEECHBUBBLE_HPP

#include <TweenEngine/TweenManager.h>
#include "GUI/Button.hpp"

namespace DrawAttack {

class SpeechBubble : public gui3ds::Button {
public:
	enum Side {
		Left,
		Right,
	};

	SpeechBubble();
	~SpeechBubble();

	void setSide(Side side);
	Side getSide() const;

	void setPosition(const cpp3ds::Vector2f& position);
	void setPosition(float x, float y);
	const cpp3ds::Vector2f& getPosition() const;

	void setColor(const cpp3ds::Color& color);
	const cpp3ds::Color& getColor() const;

	bool isVisible() const;

	void update(float delta);

	void pushString(cpp3ds::String string);

protected:
	void popStringAnimate();

private:
	static cpp3ds::Texture m_textureLeftBubble;
	static cpp3ds::Texture m_textureRightBubble;

	std::queue<cpp3ds::String> m_strings;
	TweenEngine::TweenManager m_tweenManager;

	cpp3ds::Color m_color;
	Side m_side;
	cpp3ds::Vector2f m_position;

	bool m_visible;
	bool m_busy;
};

} // namespace DrawAttack

#endif // DRAWATTACK_SPEECHBUBBLE_HPP
