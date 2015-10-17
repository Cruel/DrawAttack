#ifndef UTIL3DS_BUTTON_HPP
#define UTIL3DS_BUTTON_HPP

#include <cpp3ds/Graphics/Text.hpp>
#include <cpp3ds/Graphics/RectangleShape.hpp>
#include "../TweenTransformable.hpp"

namespace util3ds {

class Button : public cpp3ds::Drawable, public util3ds::TweenTransformable {
public:
	Button();

	bool update(const float delta);
	bool processEvent(const cpp3ds::Event& event);

	void setTexture(const cpp3ds::Texture* texture, bool resetRect = false);
	const cpp3ds::Texture* getTexture() const;

	void setTextOffset(const cpp3ds::Vector2f& offset);
	const cpp3ds::Vector2f& getTextOffset() const;

	void autoSize();
	void setSize(const cpp3ds::Vector2f& size);
	const cpp3ds::Vector2f& getSize() const;

	void setString(const cpp3ds::String& string);
	const cpp3ds::String& getString() const;

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

private:
	cpp3ds::RectangleShape m_rectangle;

	cpp3ds::Vector2f m_textOffset;
	cpp3ds::Vector2f m_padding;
	cpp3ds::FloatRect m_rect;

	cpp3ds::Text  m_text;
	cpp3ds::Color m_textColor;
	cpp3ds::Color m_textActiveColor;

	cpp3ds::Color m_backgroundColor;
	cpp3ds::Color m_backgroundActiveColor;

	std::function<void()> m_clickFunction;

	bool m_needsUpdate;
	bool m_autoSize;
	bool m_active;
};

} // namespace util3ds

#endif // UTIL3DS_BUTTON_HPP
