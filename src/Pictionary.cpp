#include <iostream>
#include <sstream>
#include <TweenEngine/Tween.h>
#include "Pictionary.hpp"

using namespace cpp3ds;
using namespace TweenEngine;

enum NetplayEvent
{
	CLIENT_COUNT,
	NEW_CLIENT,
	CLIENT_MOVED,
	CLIENT_ENDLINE,
	CLIENT_UNDO,
	CLIENT_VOICE
};

namespace Pictionary {


Pictionary::Pictionary() {
	board.create(300, 200);
	board.setPosition(20, 20);
	board.setLineThickness(1);
	board.setScale(0.5, 0.5);
//		board.addPoint(10, 10);
//		board.addPoint(120, 130);
//		board.addPoint(190, 130);
//		board.addPoint(190, 20);
//		board.addPoint(20, 150);
	if(socket.connect("192.168.1.11", 5000) == Socket::Done)
		std::cout << "Connected.";
//		board.endLine();
	textFPS.setColor(Color::Blue);
}

Pictionary::~Pictionary() {
	// Destructor called when game exits
}

void Pictionary::update(float delta) {
	static int i;
	if (i++ % 10 == 0) {
		std::ostringstream ss;
		ss.precision(3);
		ss << 1.f / delta << " fps";
		textFPS.setString(ss.str());
		textFPS.setPosition(395 - textFPS.getGlobalBounds().width, 200);
	}

	Packet packetReceive;
	while (socket.receive(packetReceive) == Socket::Done) {
		int i;
		packetReceive >> i;
		NetplayEvent event = static_cast<NetplayEvent>(i);
		switch (event) {
			case CLIENT_MOVED:
				float x, y;
				packetReceive >> x >> y;
				board.addPoint(x, y);
				break;
			case CLIENT_ENDLINE:
				board.endLine();
				break;
			default:
				break;
		}
	}

	tweenManager.update(delta);
}

void Pictionary::processEvent(Event& event) {
	if (event.type == Event::TouchMoved) {
		// TODO: take into account scale/rotation too?
		const Vector2f p = board.getPosition();
		Packet packet;
		packet << CLIENT_MOVED << event.touch.x - p.x << event.touch.y - p.y;
		socket.send(packet);
		board.addPoint(event.touch.x - p.x, event.touch.y - p.y);
	}
	if (event.type == Event::TouchEnded) {
		Packet packet;
		packet << CLIENT_ENDLINE;
		socket.send(packet);
		board.endLine();
	}
	if (event.type == Event::KeyReleased) {
		if (event.key.code == Keyboard::A) {
			std::cout << "Starting tween in 1 second...";
			board.setPosition(30, 30);
			Tween::to(board, DrawingBoard::POSITION_XY, 0.5f)
					.target(300, 200)
					.ease(TweenEquations::easeOutBounce)
//					.delay(1.0f)
					.repeatYoyo(2, 0.2f)
					.start(tweenManager);
		}
	}
}

void Pictionary::renderTopScreen(Window& window) {
	window.clear();
	window.draw(textFPS);
}

void Pictionary::renderBottomScreen(Window& window) {
	window.clear();
	window.draw(board);
}


} // namespace Pictionary
