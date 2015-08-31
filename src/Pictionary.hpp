#ifndef PICTIONARY3DS_PICTIONARY_H
#define PICTIONARY3DS_PICTIONARY_H
#include <cpp3ds/Graphics.hpp>
#include <cpp3ds/Network.hpp>
#include <TweenEngine/TweenManager.h>
#include "DrawingBoard.hpp"


namespace Pictionary {

class Pictionary: public cpp3ds::Game {
private:
	DrawingBoard board;
	cpp3ds::Text textFPS;
	cpp3ds::TcpSocket socket;
	TweenEngine::TweenManager tweenManager;
public:
	Pictionary();
	~Pictionary();
	void update(float delta);
	void processEvent(cpp3ds::Event& event);
	void renderTopScreen(cpp3ds::Window& window);
	void renderBottomScreen(cpp3ds::Window& window);
};

}

#endif //PICTIONARY3DS_PICTIONARY_H
