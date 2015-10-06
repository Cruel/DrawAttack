#include <iostream>
#include "DrawingBoard.hpp"

namespace DrawAttack {


DrawingBoard::DrawingBoard() {
	m_activeLine.setPrimitiveType(cpp3ds::Triangles);
	m_color == cpp3ds::Color::Black;
	m_timeLimit = cpp3ds::milliseconds(20);
}


void DrawingBoard::create(float width, float height) {
	m_lines.clear();

	m_size = cpp3ds::Vector2f(width, height);
	m_background.setSize(m_size);
	m_background.setFillColor(cpp3ds::Color(200, 200, 200));
	m_background.setOutlineThickness(3);
	m_background.setOutlineColor(cpp3ds::Color::White);
}


void DrawingBoard::draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const {
	// apply the entity's transform -- combine it with the one that was passed by the caller
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

	// apply the texture
//	states.texture = &m_texture;

	target.draw(m_background, states);

	for (const cpp3ds::VertexArray& line : m_lines)
		target.draw(line, states);
	target.draw(m_activeLine, states);
}


void DrawingBoard::addPoint(const cpp3ds::Vector2f &point) {
	addPoint(point.x, point.y);
}


void DrawingBoard::addPoint(float x, float y) {
	if (x < 0 || y < 0 || x > m_size.x || y > m_size.y)
		return;
	if (m_lastPoint.x == x && m_lastPoint.y == y)
		return;
//	if (m_clock.getElapsedTime() < m_timeLimit)
//		return;
	m_clock.restart();

	// If last point is set, make segment from it to this new point
	if (m_lastPoint.x != 0 || m_lastPoint.y != 0) {
		addSegment(x, y);
	}
	m_lastPoint.x = x;
	m_lastPoint.y = y;
}


void DrawingBoard::endLine() {
	if (m_activeLine.getVertexCount() > 2) {
		m_lines.push_back(m_activeLine);
	}
	m_activeLine.clear();
	m_lastPoint.x = 0;
	m_lastPoint.y = 0;
}


void DrawingBoard::addSegment(float x, float y) {
	float xdiff = x - m_lastPoint.x;
	float ydiff = y - m_lastPoint.y;
	float angle = (xdiff == 0) ? 1.57079632679489661923f : std::atan(ydiff / xdiff);

	float xoff = std::sin(angle) * m_thickness;
	float yoff = std::cos(angle) * m_thickness;

	cpp3ds::Vertex v1, v2, v3, v4;
	v1.color = v2.color = v3.color = v4.color = m_color;
	v1.position = cpp3ds::Vector2f(x + xoff, y - yoff);
	v2.position = cpp3ds::Vector2f(x - xoff, y + yoff);
	v3.position = cpp3ds::Vector2f(m_lastPoint.x + xoff, m_lastPoint.y - yoff);
	v4.position = cpp3ds::Vector2f(m_lastPoint.x - xoff, m_lastPoint.y + yoff);
	m_activeLine.append(v1);
	m_activeLine.append(v2);
	m_activeLine.append(v3);
	m_activeLine.append(v3);
	m_activeLine.append(v4);
	m_activeLine.append(v2);
}


void DrawingBoard::setLineThickness(float thickness) {
	m_thickness = thickness;
}


} // namespace DrawAttack
