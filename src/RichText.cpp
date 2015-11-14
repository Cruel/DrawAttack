// Taken from: https://github.com/skyrpex/RichText

////////////////////////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////////////////////////
#include "RichText.hpp"

#include <cpp3ds/Graphics/Font.hpp>
#include <cpp3ds/Graphics/Rect.hpp>
#include <cpp3ds/Graphics/RenderTarget.hpp>

#include <cpp3ds/System/String.hpp>

namespace util3ds
{

////////////////////////////////////////////////////////////////////////////////
void RichText::Line::setCharacterSize(unsigned int size)
{
    for (cpp3ds::Text &text : m_texts)
        text.setCharacterSize(size);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::setFont(const cpp3ds::Font &font)
{
    for (cpp3ds::Text &text : m_texts)
        text.setFont(font);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
const std::vector<cpp3ds::Text> &RichText::Line::getTexts() const
{
    return m_texts;
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::appendText(cpp3ds::Text text)
{
    // Set text offset
    updateTextAndGeometry(text);

    // Push back
    m_texts.push_back(std::move(text));
}


////////////////////////////////////////////////////////////////////////////////
cpp3ds::FloatRect RichText::Line::getLocalBounds() const
{
    return m_bounds;
}


////////////////////////////////////////////////////////////////////////////////
cpp3ds::FloatRect RichText::Line::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const
{
    states.transform *= getTransform();

    for (const cpp3ds::Text &text : m_texts)
        target.draw(text, states);
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::updateGeometry() const
{
    m_bounds = cpp3ds::FloatRect();

    for (cpp3ds::Text &text : m_texts)
        updateTextAndGeometry(text);
}


////////////////////////////////////////////////////////////////////////////////
void RichText::Line::updateTextAndGeometry(cpp3ds::Text &text) const
{
    // Set text offset
    text.setPosition(m_bounds.width, 0.f);

    // Update bounds
    int lineSpacing = text.getFont()->getLineSpacing(text.getCharacterSize());
    m_bounds.height = std::max(m_bounds.height, static_cast<float>(lineSpacing));
    m_bounds.width += text.getGlobalBounds().width;
}


////////////////////////////////////////////////////////////////////////////////
RichText::RichText()
    : RichText(nullptr)
{

}


////////////////////////////////////////////////////////////////////////////////
RichText::RichText(const cpp3ds::Font &font)
    : RichText(&font)
{

}


////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (const cpp3ds::Color &color)
{
    m_currentColor = color;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (cpp3ds::Text::Style style)
{
    m_currentStyle = style;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
std::vector<cpp3ds::String> explode(const cpp3ds::String &string, cpp3ds::Uint32 delimiter) {
    if (string.isEmpty())
        return std::vector<cpp3ds::String>();

    // For each character in the string
    std::vector<cpp3ds::String> result;
    cpp3ds::String buffer;
    for (cpp3ds::Uint32 character : string) {
        // If we've hit the delimiter character
        if (character == delimiter) {
            // Add them to the result vector
            result.push_back(buffer);
            buffer.clear();
        } else {
            // Accumulate the next character into the sequence
            buffer += character;
        }
    }

    // Add to the result if buffer still has contents or if the last character is a delimiter
    if (!buffer.isEmpty() || string[string.getSize() - 1] == delimiter)
        result.push_back(buffer);

    return result;
}


////////////////////////////////////////////////////////////////////////////////
RichText & RichText::operator << (const cpp3ds::String &string)
{
    // Maybe skip
    if (string.isEmpty())
        return *this;

    // Explode into substrings
    std::vector<cpp3ds::String> subStrings = explode(string, '\n');

    // Append first substring using the last line
    auto it = subStrings.begin();
    if (it != subStrings.end()) {
        // If there isn't any line, just create it
        if (m_lines.empty())
            m_lines.resize(1);

        // Remove last line's height
        Line &line = m_lines.back();
        m_bounds.height -= line.getGlobalBounds().height;

        // Append text
        line.appendText(createText(*it));

        // Update bounds
        m_bounds.height += line.getGlobalBounds().height;
        m_bounds.width = std::max(m_bounds.width, line.getGlobalBounds().width);
    }

    // Append the rest of substrings as new lines
    while (++it != subStrings.end()) {
        Line line;
        line.setPosition(0.f, m_bounds.height);
        line.appendText(createText(*it));
        m_lines.push_back(std::move(line));

        // Update bounds
        m_bounds.height += line.getGlobalBounds().height;
        m_bounds.width = std::max(m_bounds.width, line.getGlobalBounds().width);
    }

    // Return
    return *this;
}


////////////////////////////////////////////////////////////////////////////////
void RichText::setCharacterSize(unsigned int size)
{
    // Maybe skip
    if (m_characterSize == size)
        return;

    // Update character size
    m_characterSize = size;

    // Set texts character size
    for (Line &line : m_lines)
        line.setCharacterSize(size);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::setFont(const cpp3ds::Font &font)
{
    // Maybe skip
    if (m_font == &font)
        return;

    // Update font
    m_font = &font;

    // Set texts font
    for (Line &line : m_lines)
        line.setFont(font);

    updateGeometry();
}


////////////////////////////////////////////////////////////////////////////////
void RichText::clear()
{
    // Clear texts
    m_lines.clear();

    // Reset bounds
    m_bounds = cpp3ds::FloatRect();
}


////////////////////////////////////////////////////////////////////////////////
const std::vector<RichText::Line> &RichText::getLines() const
{
    return m_lines;
}


////////////////////////////////////////////////////////////////////////////////
unsigned int RichText::getCharacterSize() const
{
    return m_characterSize;
}


////////////////////////////////////////////////////////////////////////////////
const cpp3ds::Font *RichText::getFont() const
{
    return m_font;
}


////////////////////////////////////////////////////////////
cpp3ds::FloatRect RichText::getLocalBounds() const
{
    return m_bounds;
}


////////////////////////////////////////////////////////////
cpp3ds::FloatRect RichText::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}


////////////////////////////////////////////////////////////////////////////////
void RichText::draw(cpp3ds::RenderTarget& target, cpp3ds::RenderStates states) const
{
    states.transform *= getTransform();

    for (const Line &line : m_lines)
        target.draw(line, states);
}


////////////////////////////////////////////////////////////////////////////////
RichText::RichText(const cpp3ds::Font *font)
    : m_font(font),
      m_characterSize(30),
      m_currentColor(cpp3ds::Color::White),
      m_currentStyle(cpp3ds::Text::Regular)
{

}


////////////////////////////////////////////////////////////////////////////////
cpp3ds::Text RichText::createText(const cpp3ds::String &string) const
{
    cpp3ds::Text text;
    text.setString(string);
    text.setColor(m_currentColor);
    text.setStyle(m_currentStyle);
    text.setCharacterSize(m_characterSize);
    if (m_font)
        text.setFont(*m_font);

    return text;
}


////////////////////////////////////////////////////////////////////////////////
void RichText::updateGeometry() const
{
    m_bounds = cpp3ds::FloatRect();

    for (Line &line : m_lines) {
        line.setPosition(0.f, m_bounds.height);

        m_bounds.height += line.getGlobalBounds().height;
        m_bounds.width = std::max(m_bounds.width, line.getGlobalBounds().width);
    }
}


void RichText::setOpacity(cpp3ds::Uint8 alpha)
{
	for (auto& line : m_lines)
		line.setOpacity(alpha);
}


cpp3ds::Uint8 RichText::getOpacity() const
{
	if (m_lines.empty())
		return 0;

	return m_lines.front().getOpacity();
}


void RichText::Line::setOpacity(cpp3ds::Uint8 alpha)
{
	for (auto& text : m_texts) {
		cpp3ds::Color color = text.getColor();
		color.a = alpha;
		text.setColor(color);
	}
}


cpp3ds::Uint8 RichText::Line::getOpacity() const
{
	if (m_texts.empty())
		return 0;

	return m_texts.front().getColor().a;
}

} // namespace util3ds
