#include "DrawingBoard.hpp"

namespace pic3ds {


DrawingBoard::DrawingBoard() {
	m_vertices.setPrimitiveType(cpp3ds::TrianglesStrip);
}


void DrawingBoard::create(float width, float height) {
	m_vertices.clear();

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

	// draw the vertex array
	target.draw(m_vertices, states);
}


void DrawingBoard::addPoint(const cpp3ds::Vector2f &point) {
	addPoint(point.x, point.y);
}


void DrawingBoard::addPoint(float x, float y) {
	if (x < 0 || y < 0 || x > m_size.x || y > m_size.y)
		return;
	if (m_lastPoint.x == x && m_lastPoint.y == y)
		return;

	// If last point is set, make segment from it to this new point
	if (m_lastPoint.x != 0 || m_lastPoint.y != 0) {
		addSegment(x, y);
	}
	m_lastPoint.x = x;
	m_lastPoint.y = y;
}


void DrawingBoard::endLine() {
	if (m_lastPoint.x != 0 || m_lastPoint.y != 0) {
		addSegment(m_lastPoint.x, m_lastPoint.y);
		m_lastPoint.x = 0;
		m_lastPoint.y = 0;
	}
}


void DrawingBoard::addSegment(float x, float y) {
	float xdiff = x - m_lastPoint.x;
	float ydiff = y - m_lastPoint.y;
}


void DrawingBoard::setLineThickness(float thickness) {
	m_thickness = thickness;
}


} // namespace pic3ds
