#include "Button.hpp"


namespace util3ds {

Button::Button()
: m_autoSize(true)
, m_needsUpdate(true)
, m_active(false)
, m_padding(cpp3ds::Vector2f(10, 10))
{

}


void Button::draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const
{
	states.transform *= getTransform();
//	states.texture = &m_texture;

	target.draw(m_rectangle, states);
	if (!m_text.getString().isEmpty())
		target.draw(m_text, states);
}


void Button::setTexture(const cpp3ds::Texture* texture, bool resetRect)
{
	m_rectangle.setTexture(texture, resetRect);
}


const cpp3ds::Texture* Button::getTexture() const
{
	return m_rectangle.getTexture();
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


void Button::setSize(const cpp3ds::Vector2f& size)
{
	m_rectangle.setSize(size);
	m_autoSize = false;
}


const cpp3ds::Vector2f& Button::getSize() const
{
	return m_rectangle.getSize();
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
		}
	}

	if (event.type == cpp3ds::Event::TouchEnded)
	{
		if (m_active) {
			m_active = false;
			m_needsUpdate = true;
			if (m_clickFunction)
				m_clickFunction();
		}
	}

	if (event.type == cpp3ds::Event::TouchMoved)
	{
		if (m_active && !m_rect.contains(event.touch.x, event.touch.y)) {
			m_active = false;
			m_needsUpdate = true;
		}
	}

	return true;
}


bool Button::update(const float delta)
{
	if (m_needsUpdate) {
		if (m_autoSize) {
			cpp3ds::Vector2f size;
			size.x = m_text.getLocalBounds().width + m_padding.x * 2;
			size.y = m_text.getLocalBounds().height + m_padding.y * 2;
			setSize(size);

			m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width/2, m_text.getLocalBounds().top + m_text.getLocalBounds().height/2);
			m_text.setPosition(getSize().x/2, getSize().y/2);
		}

		if (m_active) {
			m_rectangle.setFillColor(m_backgroundActiveColor);
			m_text.setColor(m_textActiveColor);
		} else {
			m_rectangle.setFillColor(m_backgroundColor);
			m_text.setColor(m_textColor);
		}

		m_needsUpdate = false;
	}

	m_rect.left = getPosition().x;
	m_rect.top = getPosition().y;
	m_rect.width = getSize().x;
	m_rect.height = getSize().y;

	return true;
}


void Button::onClick(const std::function<void()>& callback)
{
	m_clickFunction = callback;
}


} // namespace util3ds
