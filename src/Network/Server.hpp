#ifndef DRAWATTACK_SERVER_HPP
#define DRAWATTACK_SERVER_HPP

#include <cpp3ds/Network.hpp>
#include "Player.hpp"
#include <list>

namespace DrawAttack {

class Server {
public:
	Server(unsigned short port, std::string wordFilename);
	~Server();
	void sendToAll(cpp3ds::Packet& packet);
	void sendPlayerData(cpp3ds::TcpSocket* socket);
	void sendDrawerDesignation(Player& drawer);
	void processSocket(cpp3ds::TcpSocket* socket);
	void run();
	void exit();
private:
	bool m_running;
	unsigned short m_port;
	std::string m_wordFilename;
	std::list<std::string> m_wordList;
	std::list<std::string> m_wordListUsed;
	cpp3ds::TcpListener m_listener;
	std::vector<cpp3ds::TcpSocket*> m_sockets;
	std::vector<Player> m_players;
	cpp3ds::SocketSelector m_selector;
};

} // namespace DrawAttack

#endif //DRAWATTACK_SERVER_HPP
