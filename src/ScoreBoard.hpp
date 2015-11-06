#ifndef DRAWATTACK_SCOREBOARD_HPP
#define DRAWATTACK_SCOREBOARD_HPP

#include <TweenEngine/TweenManager.h>
#include "TweenTransformable.hpp"
#include "GUI/Button.hpp"

namespace DrawAttack {

class ScoreBoard : public gui3ds::Button {
public:
	ScoreBoard();

	bool isVisible() const;

	void addText(cpp3ds::String text);
	void clear();

	void update(float delta);
	void show();
	void hide();

protected:
	void draw(cpp3ds::RenderTarget& target, cpp3ds::RenderStates states) const;

private:
	cpp3ds::Texture m_texture;
	gui3ds::Button m_overlay;
	bool m_visible;
	TweenEngine::TweenManager m_tweenManager;
	std::vector<cpp3ds::Text> m_texts;
};

} // namespace DrawAttack

#endif // DRAWATTACK_SCOREBOARD_HPP
