#ifndef GUI3DS_BUTTON_HPP
#define GUI3DS_BUTTON_HPP

#include <cpp3ds/Graphics/Text.hpp>
#include <cpp3ds/Graphics/RectangleShape.hpp>
#include "NinePatch.hpp"
#include "../TweenTransformable.hpp"

namespace gui3ds {

class Button : public NinePatch, public util3ds::TweenTransformable {
public:
	Button();

	bool processEvent(const cpp3ds::Event& event);

	void setTextOffset(const cpp3ds::Vector2f& offset);
	const cpp3ds::Vector2f& getTextOffset() const;

	void autoSize();
	void setContentSize(const cpp3ds::Vector2f& size);
	const cpp3ds::Vector2f& getSize() const;

	void setString(const cpp3ds::String& string);
	const cpp3ds::String& getString() const;

	void setFont(const cpp3ds::Font& font);
	const cpp3ds::Font* getFont() const;

	void setTextSize(unsigned int size);
	unsigned int getTextSize() const;

	void setTextColor(const cpp3ds::Color& color);
	const cpp3ds::Color& getTextColor() const;

	void setTextActiveColor(const cpp3ds::Color& color);
	const cpp3ds::Color& getTextActiveColor() const;

	void setColor(const cpp3ds::Color& color);
	const cpp3ds::Color& getColor() const;

	void setActiveColor(const cpp3ds::Color& color);
	const cpp3ds::Color& getActiveColor() const;

	void onClick(const std::function<void()>& callback);

protected:
	virtual void draw(cpp3ds::RenderTarget& target, cpp3ds::RenderStates states) const;
	void ensureUpdate() const;

private:
	mutable cpp3ds::Vector2f m_size;
	mutable cpp3ds::FloatRect m_rect;
	mutable cpp3ds::Text m_text;

	cpp3ds::Vector2f m_textOffset;
	cpp3ds::Color m_textColor;
	cpp3ds::Color m_textActiveColor;

	cpp3ds::Color m_backgroundColor;
	cpp3ds::Color m_backgroundActiveColor;

	std::function<void()> m_clickFunction;

	mutable bool m_needsUpdate;
	bool m_autoSize;
	bool m_active;
};

} // namespace gui3ds

#endif // GUI3DS_BUTTON_HPP
