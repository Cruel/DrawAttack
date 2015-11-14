#include <cpp3ds/OpenGL.hpp>
#include "ColorPickerState.hpp"

namespace DrawAttack {

ColorPickerState::ColorPickerState(StateStack &stack, State::Context &context)
: State(stack, context)
, m_picked(false)
{
	m_rect = cpp3ds::FloatRect(20.f, 20.f, 280.f, 200.f);
	m_background.setSize(cpp3ds::Vector2f(320.f, 240.f));

	m_selectorTexture.loadFromFile("images/crosshairs.png");
	m_selector.setTexture(m_selectorTexture, true);

	std::vector<cpp3ds::Color> colors = {
		cpp3ds::Color::Red,
		cpp3ds::Color::Yellow,
		cpp3ds::Color::Green,
		cpp3ds::Color::Cyan,
		cpp3ds::Color::Blue,
		cpp3ds::Color::Magenta,
		cpp3ds::Color::Red,
	};

	float colorWidth = m_rect.width / (colors.size() - 1);

	for (int i = 0; i < colors.size()-1 ; i++)
	{
		cpp3ds::Vertex vertex[6];
		vertex[0].color = vertex[1].color = cpp3ds::Color::White;
		vertex[2].color = colors[i];
		vertex[3].color = colors[i+1];
		vertex[4].color = vertex[5].color = cpp3ds::Color::Black;

		vertex[0].position.x = vertex[2].position.x = vertex[4].position.x = m_rect.left + colorWidth * i;
		vertex[1].position.x = vertex[3].position.x = vertex[5].position.x = m_rect.left + colorWidth * (i+1);

		vertex[0].position.y = vertex[1].position.y = m_rect.top;
		vertex[2].position.y = vertex[3].position.y = m_rect.top + m_rect.height / 2.f;
		vertex[4].position.y = vertex[5].position.y = m_rect.top + m_rect.height;


		// Add vertices counter-clockwise as four triangles
		m_vertices.append(vertex[0]);
		m_vertices.append(vertex[2]);
		m_vertices.append(vertex[3]);

		m_vertices.append(vertex[1]);
		m_vertices.append(vertex[0]);
		m_vertices.append(vertex[3]);

		m_vertices.append(vertex[2]);
		m_vertices.append(vertex[4]);
		m_vertices.append(vertex[5]);

		m_vertices.append(vertex[3]);
		m_vertices.append(vertex[2]);
		m_vertices.append(vertex[5]);
	}
}


void ColorPickerState::renderTopScreen(cpp3ds::Window &window)
{
	// Nothing
}


void ColorPickerState::renderBottomScreen(cpp3ds::Window &window)
{
	window.draw(m_background);
	window.draw(m_vertices);

	if (m_position != cpp3ds::Vector2f(0, 0))
	{
		GLubyte *pixels = (GLubyte*) malloc(3);
		if (pixels) {
			glReadPixels(static_cast<GLint>(m_position.x), static_cast<GLint>(m_position.y), 1, 1, GL_BGR, GL_UNSIGNED_BYTE, pixels);
			cpp3ds::Color color = cpp3ds::Color(pixels[2], pixels[1], pixels[0]);
			free(pixels);

			// Bad solution to ignore black picked up from crosshair image
			if (color != cpp3ds::Color::Black)
				m_selectedColor = color;

			// Set context color and exit this state when color is picked
			if (m_picked) {
				getContext().color = m_selectedColor;
				getContext().client.sendColor(m_selectedColor);
				requestStackPop();
			}

			m_background.setFillColor(m_selectedColor);
		}

		window.draw(m_selector);
	}
}


bool ColorPickerState::update(float delta)
{
	return true;
}


bool ColorPickerState::processEvent(const cpp3ds::Event &event)
{
	if (event.type == cpp3ds::Event::TouchMoved || event.type == cpp3ds::Event::TouchBegan) {
		if (m_rect.contains(event.touch.x, event.touch.y)) {
			m_position.x = event.touch.x;
			m_position.y = 239.f - event.touch.y;
			m_selector.setPosition(event.touch.x - 8, event.touch.y - 8);
		}
	}

	if (event.type == cpp3ds::Event::TouchEnded) {
		m_picked = true;
	}

	return false;
}

} // namespace DrawAttack
