#include "Button.hpp"


namespace gui3ds {

Button::Button()
: m_autoSize(true)
, m_needsUpdate(true)
, m_active(false)
, m_backgroundColor(cpp3ds::Color::White)
{

}


void Button::draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const
{
	ensureUpdate();

	states.transform *= getTransform();

	NinePatch::draw(target, states);
	if (!m_text.getString().isEmpty())
		target.draw(m_text, states);
}


void Button::setTextOffset(const cpp3ds::Vector2f& offset)
{
	m_textOffset = offset;
	m_needsUpdate = true;
}


const cpp3ds::Vector2f& Button::getTextOffset() const
{
	return m_textOffset;
}


void Button::setContentSize(const cpp3ds::Vector2f& size)
{
	NinePatch::setContentSize(size);
	m_autoSize = false;
}


const cpp3ds::Vector2f& Button::getSize() const
{
	ensureUpdate();
	return m_size;
}


void Button::setString(const cpp3ds::String& string)
{
	m_text.setString(string);
	m_needsUpdate = true;
}


const cpp3ds::String& Button::getString() const
{
	return m_text.getString();
}


void Button::setFont(const cpp3ds::Font& font)
{
	m_text.setFont(font);
}


const cpp3ds::Font* Button::getFont() const
{
	return m_text.getFont();
}


void Button::setTextSize(unsigned int size)
{
	m_text.setCharacterSize(size);
}


unsigned int Button::getTextSize() const
{
	return m_text.getCharacterSize();
}


void Button::setTextColor(const cpp3ds::Color& color)
{
	m_textColor = color;
	m_needsUpdate = true;
}


const cpp3ds::Color& Button::getTextColor() const
{
	return m_textColor;
}


void Button::setTextActiveColor(const cpp3ds::Color& color)
{
	m_textActiveColor = color;
	m_needsUpdate = true;
}


const cpp3ds::Color& Button::getTextActiveColor() const
{
	return m_textActiveColor;
}


void Button::setColor(const cpp3ds::Color& color)
{
	m_backgroundColor = color;
	m_needsUpdate = true;
}


const cpp3ds::Color& Button::getColor() const
{
	return m_backgroundColor;
}


void Button::setActiveColor(const cpp3ds::Color& color)
{
	m_backgroundActiveColor = color;
	m_needsUpdate = true;
}


const cpp3ds::Color& Button::getActiveColor() const
{
	return m_backgroundActiveColor;
}


void Button::autoSize()
{
	m_autoSize = true;
	m_needsUpdate = true;
}


bool Button::processEvent(const cpp3ds::Event &event)
{
	if (event.type == cpp3ds::Event::TouchBegan)
	{
		if (m_rect.contains(event.touch.x, event.touch.y))
		{
			m_active = true;
			m_needsUpdate = true;
			return false;
		}
	}

	if (event.type == cpp3ds::Event::TouchEnded)
	{
		if (m_active) {
			m_active = false;
			m_needsUpdate = true;
			if (m_clickFunction)
				m_clickFunction();
			return false;
		}
	}

	if (event.type == cpp3ds::Event::TouchMoved)
	{
		if (m_active) {
			if (!m_rect.contains(event.touch.x, event.touch.y)) {
				m_active = false;
				m_needsUpdate = true;
			}
			return false;
		}
	}

	return true;
}


void Button::ensureUpdate() const
{
	if (m_needsUpdate)
	{
		if (m_autoSize) {
			NinePatch::setContentSize(cpp3ds::Vector2f(m_text.getLocalBounds().width, m_text.getLocalBounds().height));

			cpp3ds::FloatRect textBounds = m_text.getLocalBounds();
			cpp3ds::FloatRect padding = getPadding();
			cpp3ds::Vector2f contentSize = getContentSize();

			m_size.x = contentSize.x + getTexture()->getSize().x - padding.width;
			m_size.y = contentSize.y + getTexture()->getSize().y - padding.height;

			m_text.setOrigin(std::floor(textBounds.left + textBounds.width/2),
			                 std::floor(textBounds.top + textBounds.height/2));
			m_text.setPosition(std::floor(padding.left + contentSize.x/2 + m_textOffset.x),
			                   std::floor(padding.top + contentSize.y/2 + m_textOffset.y));
		}

		if (m_active) {
			NinePatch::setColor(m_backgroundActiveColor);
			m_text.setColor(m_textActiveColor);
		} else {
			NinePatch::setColor(m_backgroundColor);
			m_text.setColor(m_textColor);
		}

		m_needsUpdate = false;
	}

	m_rect.left = getPosition().x;
	m_rect.top = getPosition().y;
	m_rect.width = m_size.x;
	m_rect.height = m_size.y;
}


void Button::onClick(const std::function<void()>& callback)
{
	m_clickFunction = callback;
}


} // namespace gui3ds
