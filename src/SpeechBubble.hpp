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
	static const int COLOR_RGB   = 11;
	static const int COLOR_ALPHA = 12;
	static const int CONTENT_X   = 13;
	static const int TEXTCOLOR_ALPHA = 14;

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
	virtual int getValues(int tweenType, float *returnValues);
	virtual void setValues(int tweenType, float *newValues);
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
