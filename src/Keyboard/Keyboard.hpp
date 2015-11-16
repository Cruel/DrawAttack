#ifndef UTIL3DS_KEYBOARD_HPP
#define UTIL3DS_KEYBOARD_HPP

#include <cpp3ds/Graphics.hpp>
#include "tinyxml2.h"
#include "../TweenTransformable.hpp"

namespace util3ds {

class Keyboard: public cpp3ds::Drawable, public TweenTransformable {
public:
	Keyboard();
	void loadFromFile(const std::string& filename);
	bool processEvents(const cpp3ds::Event& event);
	void update(float delta);
	bool popString(cpp3ds::String& string);

protected:
	void draw(cpp3ds::RenderTarget& target, cpp3ds::RenderStates states) const;
	void updateVertices();
	void enterText();

private:
	struct State {
		State(): fontSize(12), offsetX(6), offsetY(5) {}
		unsigned int fontSize;
		int offsetX;
		int offsetY;
		cpp3ds::Color color;
		cpp3ds::IntRect textureRect;
	};
	struct Style {
		std::string name;
		State defaultState;
		State activeState;
	};
	enum ButtonType {
		Key,
		Enter,
		Backspace,
		LayoutMenu,
		LayoutChange,
		LayoutChangeTemp,
	};
	struct Button {
		ButtonType      type;
		cpp3ds::String  data;
		unsigned char   styleIndex;
		cpp3ds::IntRect rect;
		cpp3ds::Text    text;
	};
	struct Layout {
		std::string         name;
		std::string         label;
		std::vector<Button> buttons;
	};
	struct Input : Button {
		Input(): maxWidth(200), cursorPosition(0) {}
		int maxWidth;
		int cursorPosition;
	};

	void updateButtonVertices(Button& button);
	bool fillState(State& state, const tinyxml2::XMLElement* styleNode, const char* stateName);
	void loadStyles(const tinyxml2::XMLElement* styleNode);
	void loadButtons(Layout& layout, const tinyxml2::XMLElement* layoutNode);

	cpp3ds::Texture       m_texture;
	cpp3ds::VertexArray   m_vertices;
	cpp3ds::Font          m_font;
	bool                  m_usingFont;

	tinyxml2::XMLDocument  m_xml;
	std::vector<Layout>    m_layouts;
	std::vector<Style>     m_styles;
	Button*                m_activeButton;
	Input                  m_input;
	cpp3ds::RectangleShape m_cursor;
	mutable cpp3ds::Clock  m_cursorClock;

	std::string           m_filename;
	unsigned char         m_layoutIndex;
	unsigned char         m_tempLayoutIndex;
	bool                  m_usingTempLayout;
	bool                  m_loaded;
	bool                  m_needsUpdate;

	std::queue<cpp3ds::String> m_strings;
};

} // namesapce util3ds

#endif // UTIL3DS_KEYBOARD_HPP
