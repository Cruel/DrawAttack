#include <TweenEngine/Tween.h>
#include "ChatLog.hpp"

namespace DrawAttack {

ChatLog::ChatLog()
: m_visible(false)
, m_scrollPosition(0.f)
, m_characterSize(1)
, m_lineHeight(1.f)
, m_spacing(0.f)
{
	m_overlay.setContentSize(cpp3ds::Vector2f(400.f, 240.f));
	m_overlay.setColor(cpp3ds::Color::Transparent);
}

void ChatLog::setCharacterSize(unsigned int size)
{
	m_characterSize = (size == 0) ? 1 : size;

	util3ds::RichText text;
	text.clear();
	text.setCharacterSize(m_characterSize);
	text << " " << " "; // Hack call twice, first to intialize cpp3ds::Text default font
	m_lineHeight = text.getLocalBounds().height;

	m_texts.resize(240 / m_lineHeight + 2);
	for (auto& text : m_texts) {
		text.setCharacterSize(m_characterSize);
	}

	updateLines();
}

void ChatLog::addLine(const std::string& name, const std::string& text)
{
	ChatLine line(name, text);
	if (m_lines.size() >= 500)
		m_lines.pop_front();

	m_lines.emplace_back(line);

	int fullHeight = m_lines.size() * (m_lineHeight + m_spacing);
	if (fullHeight - m_scrollPosition < (m_lineHeight + m_spacing) * 4)
		m_scrollPosition = fullHeight;

	updateLines();
}

void ChatLog::draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const
{
	target.draw(m_overlay);

	if (m_lines.size() > 0) {
		states.transform *= getTransform();
		for (auto& text : m_texts) {
			if (!text.getLines().empty())
				target.draw(text, states);
		}
	}
}

bool ChatLog::isVisible() const
{
	return m_visible;
}


void ChatLog::show()
{
	m_visible = true;
	m_tweenManager.killAll();
	TweenEngine::Tween::to(*this, ALPHA, 0.3f).target(1.f).start(m_tweenManager);
}


void ChatLog::hide()
{
	m_tweenManager.killAll();

	TweenEngine::Tween::to(*this, ALPHA, 0.3f)
		.target(0.f)
		.setCallback(TweenEngine::TweenCallback::COMPLETE, [this](TweenEngine::BaseTween* source){
			m_visible = false;
		})
		.start(m_tweenManager);
}


void ChatLog::update(float delta)
{
	m_tweenManager.update(delta);
}


void ChatLog::clear()
{
	m_lines.clear();
}


void ChatLog::setScrollPosition(float position)
{
	if (m_scrollPosition == position || m_lines.size() == 0)
		return;

	int fullHeight = m_lines.size() * (m_lineHeight + m_spacing);
	if (position > fullHeight)
		position = fullHeight;

	m_scrollPosition = position;

	updateLines();
}

void ChatLog::moveScrollPosition(float offset)
{
	setScrollPosition(std::max(0.f, m_scrollPosition + offset));
}

float ChatLog::getScrollPosition() const
{
	return m_scrollPosition;
}

void ChatLog::updateLines()
{
	float lineHeight = m_lineHeight + m_spacing;
	int i = (m_scrollPosition) / lineHeight - 1;
	float offset = m_scrollPosition - (lineHeight * i);
	i -= 240.f / lineHeight;
	offset -= 240.f - std::floor(240.f / lineHeight) * lineHeight;
	if (m_scrollPosition >= 240.f + lineHeight)
		i++;

	int j = 0;

	for (auto& text : m_texts) {
		text.setPosition(5.f, static_cast<float>(j) * lineHeight - offset);
		if (i >= 0 && i < m_lines.size()) {
			text.clear();
			text << cpp3ds::Text::Bold << ChatLine::colorMap[m_lines[i].name] << m_lines[i].name << "  "
			     << cpp3ds::Text::Regular << cpp3ds::Color::White << m_lines[i].text;
		} else {
			text.clear();
		}
		i++;
		j++;
	}
}


int ChatLog::getValues(int tweenType, float *returnValues)
{
	switch (tweenType) {
		case ALPHA:
			returnValues[0] = m_opacity;
			return 1;
		default:
			return TweenTransformable::getValues(tweenType, returnValues);
	}
}


void ChatLog::setValues(int tweenType, float *newValues)
{
	switch (tweenType) {
		case ALPHA: setOpacity(newValues[0]); break;
		default:
			TweenTransformable::setValues(tweenType, newValues);
	}
}


void ChatLog::setOpacity(float alpha)
{
	m_opacity = alpha;

	for (auto& text : m_texts)
		text.setOpacity(alpha * 255);

	cpp3ds::Color color = m_overlay.getColor();
	color.a = alpha * 180;
	m_overlay.setColor(color);
}


float ChatLog::getOpacity() const
{
	return m_opacity;
}


int ChatLog::ChatLine::colorIndex = 0;

std::map<std::string, cpp3ds::Color> ChatLog::ChatLine::colorMap;

std::vector<cpp3ds::Color> ChatLog::ChatLine::colorList = {
	cpp3ds::Color(0,245,245),
	cpp3ds::Color(255,100,100),
	cpp3ds::Color(155,155,255),
	cpp3ds::Color(155,255,155),
	cpp3ds::Color(255,100,255),
	cpp3ds::Color(0,245,0),
	cpp3ds::Color(245,245,0),
};

ChatLog::ChatLine::ChatLine(const std::string &name, const std::string &text)
: name(name)
, text(text)
{
	if (colorMap.find(name) == colorMap.end()) {
		colorIndex %= colorList.size();
		colorMap[name] = colorList[colorIndex];
		colorIndex++;
//		color = colorList[colorIndex];
	}
//	color = cpp3ds::Color::Green;
//	std::cout << colorIndex << " " << (int)color.a  << " " << colorList.size() << std::endl;
}

} // namespace DrawAttack
