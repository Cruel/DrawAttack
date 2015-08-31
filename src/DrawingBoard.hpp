#ifndef PICTIONARY3DS_DRAWINGBOARD_H
#define PICTIONARY3DS_DRAWINGBOARD_H

#include <cpp3ds/Graphics.hpp>
#include "TweenTransformable.hpp"

namespace Pictionary {

class DrawingBoard : public cpp3ds::Drawable, public util3ds::TweenTransformable {

public:

	DrawingBoard();
	void create(float width, float height);

	void addPoint(float x, float y);
	void addPoint(const cpp3ds::Vector2f& position);

	void setLineThickness(float thickness);

	void endLine();

private:

	virtual void draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const;

	void addSegment(float x, float y);

	cpp3ds::Vector2f m_size;
	float m_thickness;
	cpp3ds::Color m_color;
	cpp3ds::Vector2f m_lastPoint;
	cpp3ds::RectangleShape m_background;
	std::vector<cpp3ds::VertexArray> m_lines;
	cpp3ds::VertexArray m_activeLine;
	cpp3ds::Texture m_texture;

	cpp3ds::Time m_timeLimit;
	cpp3ds::Clock m_clock;

};

} // namespace pic3ds


#endif //PICTIONARY3DS_DRAWINGBOARD_H
