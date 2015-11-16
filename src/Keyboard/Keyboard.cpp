#include <cpp3ds/System/FileSystem.hpp>
#include "Keyboard.hpp"

using namespace tinyxml2;

namespace util3ds {


Keyboard::Keyboard() {
	m_loaded = false;
	m_activeButton = NULL;
	m_usingTempLayout = false;
	m_needsUpdate = false;
}


void Keyboard::loadFromFile(const std::string &filename)
{
	m_loaded = false;
	m_activeButton = NULL;
	m_filename = cpp3ds::FileSystem::getFilePath(filename);
	if (m_xml.LoadFile(m_filename.c_str()) != XML_NO_ERROR)
		return;

	// Keyboard element
	const XMLElement* keyboard = m_xml.FirstChildElement("keyboard");
	if (keyboard == NULL) {
		cpp3ds::err() << "No \"keyboard\" element defined in: " << filename << std::endl;
		return;
	}
	const char* defaultLayoutName = keyboard->Attribute("default-layout");
	const char* image = keyboard->Attribute("image");
	const char* font = keyboard->Attribute("font");
	std::string path = m_filename.substr(0, m_filename.find_last_of("/"));
	if (image)
		m_texture.loadFromFile(path + "/" + image);
	m_usingFont = font != NULL;
	if (m_usingFont)
		m_usingFont = m_font.loadFromFile(path + "/" + font);

	// Style elements
	const XMLElement* styleNode = keyboard->FirstChildElement("style");
	if (styleNode == NULL) {
		cpp3ds::err() << "At least one \"style\" element needed in: " << filename << std::endl;
		return;
	}
	loadStyles(styleNode);

	// Layout element(s)
	m_layouts.clear();
	const XMLElement* layoutNode = keyboard->FirstChildElement("layout");
	if (layoutNode == NULL) {
		cpp3ds::err() << "At least one \"layout\" element needed in: " << filename << std::endl;
		return;
	}
	while (layoutNode != NULL) {
		Layout layout;
		layout.name = layoutNode->Attribute("name");
		layout.label = layoutNode->Attribute("label");
		if (!layout.name.empty()) {
			if (layout.name == defaultLayoutName)
				m_layoutIndex = m_layouts.size();

			// Load all Button elements
			loadButtons(layout, layoutNode);

			m_layouts.push_back(layout);
		} else {
			cpp3ds::err() << "Layout element has no \"name\" attribute. Skipping it." << std::endl;
		}
		layoutNode = layoutNode->NextSiblingElement("layout");
	}

	// Input element
	const XMLElement* inputNode = keyboard->FirstChildElement("input");
	if (inputNode == NULL) {
		cpp3ds::err() << "No \"input\" element defined in: " << filename << std::endl;
		return;
	} else {
		const char* styleName = inputNode->Attribute("style");
		int x = inputNode->IntAttribute("x");
		int y = inputNode->IntAttribute("y");
		int w = inputNode->IntAttribute("w");
		int h = inputNode->IntAttribute("h");
		m_input.rect = cpp3ds::IntRect(x, y, w, h);
		inputNode->QueryIntAttribute("max-width", &m_input.maxWidth);

		float cursorHeight = 12.f;
		float cursorWidth = 1.f;
		inputNode->QueryFloatAttribute("cursor-height", &cursorHeight);
		inputNode->QueryFloatAttribute("cursor-width", &cursorWidth);
		m_cursor.setSize(cpp3ds::Vector2f(cursorWidth, cursorHeight));
		m_cursor.setFillColor(cpp3ds::Color::White);
		m_cursor.setPosition(0, y + (h - cursorHeight) / 2);

		m_input.styleIndex = 0;
		for (const Style& style: m_styles) {
			if (style.name == styleName)
				break;
			m_input.styleIndex++;
		}
	}

	m_loaded = true;
	updateVertices();
}


bool Keyboard::fillState(State& state, const XMLElement* styleNode, const char* stateName)
{
	const XMLElement* stateNode = styleNode->FirstChildElement("state");
	while (stateNode != NULL) {
		if (strcmp(stateName, stateNode->Attribute("name")) == 0)
			break;
		stateNode = stateNode->NextSiblingElement("state");
	}
	if (stateNode == NULL)
		return false;

	const XMLElement* textNode = stateNode->FirstChildElement("text");
	if (textNode != NULL) {
		textNode->QueryUnsignedAttribute("fontsize", &state.fontSize);
		textNode->QueryIntAttribute("offsetx", &state.offsetX);
		textNode->QueryIntAttribute("offsety", &state.offsetY);
		state.color = cpp3ds::Color::Black;
	}
	const XMLElement* imageNode = stateNode->FirstChildElement("image");
	if (imageNode != NULL) {
		int x = imageNode->IntAttribute("x");
		int y = imageNode->IntAttribute("y");
		int w = imageNode->IntAttribute("w");
		int h = imageNode->IntAttribute("h");
		state.textureRect = cpp3ds::IntRect(x, y, w, h);
	}
	return true;
}


void Keyboard::loadStyles(const XMLElement* styleNode)
{
	m_styles.clear();

	while (styleNode != NULL) {
		Style style;
		style.name = styleNode->Attribute("name");
		if (!style.name.empty()) {
			if (!fillState(style.defaultState, styleNode, "default")) {
				cpp3ds::err() << "Style element has no State element named \"default\". Skipping it." << std::endl;
			} else {
				if (!fillState(style.activeState, styleNode, "active"))
					style.activeState = style.defaultState;
				m_styles.push_back(style);
			}
		} else {
			cpp3ds::err() << "Style element has no \"name\" attribute. Skipping it." << std::endl;
		}
		styleNode = styleNode->NextSiblingElement("style");
	}
}


void Keyboard::loadButtons(Layout& layout, const XMLElement* layoutNode) {
	const XMLElement *buttonNode = layoutNode->FirstChildElement("button");
	while (buttonNode != NULL) {
		const char* styleName = buttonNode->Attribute("style");
		if (styleName != NULL) {
			Button button;
			int x = buttonNode->IntAttribute("x");
			int y = buttonNode->IntAttribute("y");
			int w = buttonNode->IntAttribute("w");
			int h = buttonNode->IntAttribute("h");

			button.styleIndex = 0;
			for (const Style& style: m_styles) {
				if (style.name == styleName)
					break;
				button.styleIndex++;
			}

			if (button.styleIndex < m_styles.size()) {
				if (buttonNode->GetText() != NULL) {
					std::string stringUtf8 = buttonNode->GetText();
					std::wstring stringUtf32;
					cpp3ds::Utf8::toUtf32(stringUtf8.begin(), stringUtf8.end(), std::back_inserter(stringUtf32));
					button.data = stringUtf32;
					button.text.setString(button.data);
				}

				button.type = Key;
				const char* type = buttonNode->Attribute("type");
				if (type != NULL) {
					if (strcmp(type, "layoutmenu") == 0) {
						button.type = LayoutMenu;
					} else if (strcmp(type, "layoutchange") == 0) {
						button.type = LayoutChange;
						button.data = buttonNode->Attribute("layout");
					} else if (strcmp(type, "layoutchangetemp") == 0) {
						button.type = LayoutChangeTemp;
						button.data = buttonNode->Attribute("layout");
					} else if (strcmp(type, "enter") == 0) {
						button.type = Enter;
					} else if (strcmp(type, "backspace") == 0) {
						button.type = Backspace;
					}
				}

				button.rect = cpp3ds::IntRect(x, y, w, h);
				if (m_usingFont)
					button.text.setFont(m_font);
				layout.buttons.push_back(button);
			} else {
				cpp3ds::err() << "Key style \"" << styleName << "\" not found. Skipping the key." << std::endl;
			}
		} else {
			cpp3ds::err() << "Key element has no \"style\" attribute. Skipping it." << std::endl;
		}
		buttonNode = buttonNode->NextSiblingElement("button");
	}
}


void Keyboard::updateButtonVertices(Button& button)
{
	const Style& style = m_styles[button.styleIndex];
	const State& state = &button == m_activeButton ? style.activeState : style.defaultState;

	button.text.setCharacterSize(state.fontSize);
	button.text.setColor(state.color);
	button.text.setPosition(button.rect.left + state.offsetX, button.rect.top + state.offsetY);

	cpp3ds::Vertex vertices[4];

	vertices[0].position = cpp3ds::Vector2f(button.rect.left, button.rect.top);
	vertices[1].position = cpp3ds::Vector2f(button.rect.left + button.rect.width, button.rect.top);
	vertices[2].position = cpp3ds::Vector2f(button.rect.left, button.rect.top + button.rect.height);
	vertices[3].position = cpp3ds::Vector2f(button.rect.left + button.rect.width, button.rect.top + button.rect.height);

	vertices[0].texCoords = cpp3ds::Vector2f(state.textureRect.left, state.textureRect.top);
	vertices[1].texCoords = cpp3ds::Vector2f(state.textureRect.left + state.textureRect.width, state.textureRect.top);
	vertices[2].texCoords = cpp3ds::Vector2f(state.textureRect.left, state.textureRect.top + state.textureRect.height);
	vertices[3].texCoords = cpp3ds::Vector2f(state.textureRect.left + state.textureRect.width, state.textureRect.top + state.textureRect.height);

	m_vertices.append(vertices[0]);
	m_vertices.append(vertices[2]);
	m_vertices.append(vertices[3]);
	m_vertices.append(vertices[1]);
	m_vertices.append(vertices[0]);
	m_vertices.append(vertices[3]);
}


void Keyboard::updateVertices()
{
	m_vertices.clear();

	Layout& layout = m_layouts[m_usingTempLayout ? m_tempLayoutIndex: m_layoutIndex];

	for (Button& button: layout.buttons)
	{
		updateButtonVertices(button);
		if (button.type == LayoutMenu)
			button.text.setString(layout.label);
	}

	updateButtonVertices(m_input);

	// Update input text position
	const Style& style = m_styles[m_input.styleIndex];
	m_input.text.setColor(cpp3ds::Color::White);
	m_input.text.setPosition(static_cast<int>(160.f + style.defaultState.offsetX - m_input.text.getGlobalBounds().width / 2),
							 m_input.text.getPosition().y);

	// Update cursor position
	cpp3ds::Text dummyText(m_input.data.substring(m_input.data.getSize() - m_input.cursorPosition, m_input.cursorPosition),
	                       *m_input.text.getFont(),
	                       m_input.text.getCharacterSize());
	m_cursor.setPosition(m_input.text.getPosition().x + m_input.text.getGlobalBounds().width - dummyText.getLocalBounds().width,
	                     m_cursor.getPosition().y);

	m_needsUpdate = false;
}


bool Keyboard::processEvents(const cpp3ds::Event &event)
{
	if (!m_loaded)
		return true;

	Layout& layout = m_layouts[m_usingTempLayout ? m_tempLayoutIndex: m_layoutIndex];

	if (event.type == cpp3ds::Event::TouchBegan) {
		for (Button& button: layout.buttons) {
			if (button.rect.contains(event.touch.x, event.touch.y)) {
				m_activeButton = &button;
			}
		}

		// Check for cursor position change
		const float cursorPadding = 6.f;
		cpp3ds::FloatRect inputBounds = m_input.text.getGlobalBounds();
		inputBounds.left -= cursorPadding;
		inputBounds.width += cursorPadding * 2;
		if (inputBounds.contains(event.touch.x, event.touch.y)) {
			cpp3ds::String inputString = m_input.text.getString();
			cpp3ds::Text testText("", *m_input.text.getFont(), m_input.text.getCharacterSize());
			float clickedPosition = inputBounds.left + inputBounds.width - event.touch.x - cursorPadding + 1.f;
			int i;
			for (i = 0; i <= inputString.getSize(); i++) {
				testText.setString(inputString.substring(inputString.getSize() - i, i));
				float width = testText.getLocalBounds().width;
				if (width > clickedPosition) {
					if (i == 0)
						break;
					testText.setString(inputString.substring(inputString.getSize() - i, 1));
					if (clickedPosition > width - testText.getLocalBounds().width / 2.f) {
						break;
					} else {
						i--;
						break;
					}
				}
			}
			if (i > inputString.getSize())
				i = inputString.getSize();
			m_input.cursorPosition = i;
		}
		m_needsUpdate = true;
	}

	if (event.type == cpp3ds::Event::TouchEnded) {
		if (m_activeButton != NULL) {
			m_usingTempLayout = false;
			switch (m_activeButton->type) {
				case Enter:
					enterText();
					break;
				case Backspace:
					if (m_input.cursorPosition == m_input.data.getSize())
						break;
					m_input.data.erase(m_input.data.getSize() - m_input.cursorPosition - 1, 1);
					m_input.text.setString(m_input.data);
					break;
				case LayoutChange:
					m_layoutIndex = 0;
					for (const Layout& layout: m_layouts) {
						if (layout.name == m_activeButton->data)
							break;
						m_layoutIndex++;
					}
					break;
				case LayoutChangeTemp:
					m_usingTempLayout = true;
					m_tempLayoutIndex = 0;
					for (const Layout& layout: m_layouts) {
						if (layout.name == m_activeButton->data)
							break;
						m_tempLayoutIndex++;
					}
					break;
				default:
					if (m_input.text.getGlobalBounds().width < m_input.maxWidth) {
						m_input.data.insert(m_input.data.getSize() - m_input.cursorPosition, m_activeButton->data);
						m_input.text.setString(m_input.data);
					}
			}
			m_activeButton = NULL;
			m_needsUpdate = true;
		}
	}

	if (event.type == cpp3ds::Event::TouchMoved) {
		if (m_activeButton != NULL && !m_activeButton->rect.contains(event.touch.x, event.touch.y)) {
			m_activeButton = NULL;
			m_needsUpdate = true;
		}
	}

	if (event.type == cpp3ds::Event::KeyPressed) {
		if (event.key.code == cpp3ds::Keyboard::A) {
			enterText();
			return false;
		}
	}

	return true;
}


void Keyboard::update(float delta)
{
	if (m_needsUpdate)
		updateVertices();
}


void Keyboard::draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const
{
	if (!m_loaded)
		return;

	states.transform *= getTransform();
	states.texture = &m_texture;

	target.draw(m_vertices, states);
	states.texture = nullptr;

	const Layout& layout = m_layouts[m_usingTempLayout ? m_tempLayoutIndex: m_layoutIndex];
	for (const Button& button: layout.buttons) {
		if (!button.text.getString().isEmpty())
			target.draw(button.text, states);
	}
	target.draw(m_input.text, states);
	if (m_cursorClock.getElapsedTime() > cpp3ds::seconds(1.f))
		m_cursorClock.restart();
	else if (m_cursorClock.getElapsedTime() > cpp3ds::seconds(0.5f))
		target.draw(m_cursor, states);
}


bool Keyboard::popString(cpp3ds::String& string)
{
	if (m_strings.empty())
		return false;
	string = m_strings.front();
	m_strings.pop();
	return true;
}


void Keyboard::enterText()
{
	if (!m_input.data.isEmpty()) {
		m_strings.push(m_input.data);
		m_input.data.clear();
		m_input.text.setString(m_input.data);
		m_input.cursorPosition = 0;
	}
}

} // namesapce util3ds
