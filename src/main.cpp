#include <cpp3ds/Graphics.hpp>
#include <cpp3ds/Window.hpp>
#include "DrawingBoard.hpp"

using namespace cpp3ds;

// Minimal derived Game class necessary for cpp3ds games
class MyGame: public Game {
private:
	Text text;
	pic3ds::DrawingBoard board;
public:
	MyGame() {
		// Constructor for setting up initial game objects
		text.setColor(Color::Green);
		text.setCharacterSize(40);
		text.setString("Hello World");

		board.create(200, 100);
		board.setPosition(20, 20);
		board.setLineThickness(5);
		board.addPoint(10, 10);
		board.addPoint(20, 30);
	}

	~MyGame() {
		// Destructor called when game exits
	}

	void update(float delta) {
		// Update game state based on frame time delta
	}

	void processEvent(Event& event) {
		// Check for input Events here
	}

	void renderTopScreen(Window& window) {
		window.clear();
		window.draw(board);
		window.draw(text);
	}

	void renderBottomScreen(Window& window) {
		window.clear();
	}
};

int main(int argc, char** argv) {
	MyGame game;
	game.console(BottomScreen); // Console for reading stdout
	game.run();
	return 0;
}
