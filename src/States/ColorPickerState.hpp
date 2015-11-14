#ifndef DRAWATTACK_COLORPICKERSTATE_HPP
#define DRAWATTACK_COLORPICKERSTATE_HPP

#include <cpp3ds/Graphics/VertexArray.hpp>
#include <cpp3ds/Window/Window.hpp>
#include <cpp3ds/Graphics/Sprite.hpp>
#include <cpp3ds/Graphics/RectangleShape.hpp>
#include "State.hpp"

namespace DrawAttack {

class ColorPickerState : public State
{
public:
	ColorPickerState(StateStack& stack, Context& context);

	virtual void renderTopScreen(cpp3ds::Window& window);
	virtual void renderBottomScreen(cpp3ds::Window& window);
	virtual bool update(float delta);
	virtual bool processEvent(const cpp3ds::Event& event);

private:
	cpp3ds::RectangleShape m_background;
	cpp3ds::Texture m_selectorTexture;
	cpp3ds::Sprite m_selector;
	cpp3ds::VertexArray m_vertices;
	cpp3ds::Vector2f m_position;
	cpp3ds::Color m_selectedColor;
	cpp3ds::FloatRect m_rect;
	bool m_picked;
};

} // namespace DrawAttack

#endif // DRAWATTACK_COLORPICKERSTATE_HPP
