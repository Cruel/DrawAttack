#ifndef DRAWATTACK_CHATLOG_HPP
#define DRAWATTACK_CHATLOG_HPP

#include <cpp3ds/Graphics/RenderTarget.hpp>
#include <cpp3ds/Graphics/RenderStates.hpp>
#include <cpp3ds/Graphics/Text.hpp>
#include <list>
#include <TweenEngine/TweenManager.h>
#include <cpp3ds/Graphics/RectangleShape.hpp>
#include "TweenTransformable.hpp"
#include "RichText.hpp"
#include "GUI/Button.hpp"

namespace DrawAttack {

class ChatLog : public cpp3ds::Drawable, public util3ds::TweenTransformable {
public:
	static const int ALPHA = 11;

	struct ChatLine {
		ChatLine(const std::string& name, const std::string& text);
		std::string name;
		std::string text;
//		cpp3ds::Color color;
		static int colorIndex;
		static std::vector<cpp3ds::Color> colorList;
		static std::map<std::string, cpp3ds::Color> colorMap;
	};

	ChatLog();
	void setCharacterSize(unsigned int size);
	void setScrollPosition(float position);
	void moveScrollPosition(float offset);
	float getScrollPosition() const;
	void addLine(const std::string& name, const std::string& text);

	void updateLines();

	bool isVisible() const;

	void clear();

	void update(float delta);
	void show();
	void hide();

	void setOpacity(float alpha);
	float getOpacity() const;

protected:
	virtual void draw(cpp3ds::RenderTarget& target, cpp3ds::RenderStates states) const;

	virtual int getValues(int tweenType, float *returnValues);
	virtual void setValues(int tweenType, float *newValues);

private:
	bool m_visible;
	float m_scrollPosition;
	float m_opacity;
	float m_spacing;
	float m_lineHeight;
	unsigned int m_characterSize;
	gui3ds::Button m_overlay;
	std::vector<util3ds::RichText> m_texts;
	std::deque<ChatLine> m_lines;
	TweenEngine::TweenManager m_tweenManager;
};

} // namespace DrawAttack

#endif // DRAWATTACK_CHATLOG_HPP
