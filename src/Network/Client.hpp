#ifndef DRAWATTACK_CLIENT_HPP
#define DRAWATTACK_CLIENT_HPP

#include <cpp3ds/Network.hpp>
#include "NetworkEvents.hpp"

namespace DrawAttack {


class Client {
public:
	Client();
	~Client();
	bool pollEvent(NetworkEvent& event);
	cpp3ds::Socket::Status connect(cpp3ds::IpAddress ip, unsigned short port);
	void sendText(cpp3ds::String text);
	void sendPlayerConnected(cpp3ds::String name);
	void sendDrawMove(int x, int y);
	void sendDrawEndline(int x, int y);
private:
	cpp3ds::TcpSocket m_socket;
};


}

#endif //DRAWATTACK_CLIENT_HPP
