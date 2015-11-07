#ifndef DRAWATTACK_SERVER_HPP
#define DRAWATTACK_SERVER_HPP

#include <cpp3ds/Network.hpp>
#include "Player.hpp"
#include "NetworkEvents.hpp"
#include <list>

#define MIN_PLAYERS 3.f
#define SERVER_VERSION "1.0b"
#define MAX_PLAYERS 16.f
#define ROUND_DURATION 60.f
#define ROUND_INTERMISSION 8.f
#define ROUND_TIMEOUT 15.f
#define PING_TIMEOUT 15.f

namespace DrawAttack {

class Server {
public:
	enum Mode {
		Wait,
		Play,
	};

	Server(unsigned short port, std::string wordFilename);
	~Server();
	void sendToAllSockets(cpp3ds::Packet& packet);
	void sendToAllPlayers(cpp3ds::Packet& packet);
	void processSocket(cpp3ds::TcpSocket* socket);
	bool validateEvent(cpp3ds::TcpSocket* socket, const NetworkEvent& event);
	void run();
	void exit(std::string reason = std::string());
	void removeSocket(cpp3ds::TcpSocket* socket);

	void sendWaitForPlayers(cpp3ds::TcpSocket* socket, float playersNeeded);
	void sendPlayerData(cpp3ds::TcpSocket* socket);
	void sendDrawData(cpp3ds::TcpSocket* socket);

	void startRound(Player& drawer, std::string word, float duration);

	const Player& getNextDrawer();
	std::string getNextWord();

private:
	void clearDrawData();

	Mode m_mode;
	cpp3ds::TcpSocket* m_currentDrawer;

	bool m_running;
	unsigned short m_port;

	std::string m_wordFilename;
	std::list<std::string> m_wordList;
	std::list<std::string> m_wordListUsed;
	std::string m_currentWord;
	float m_roundDuration;
	cpp3ds::Clock m_roundClock;
	cpp3ds::Clock m_roundTimeoutClock;

	cpp3ds::TcpListener m_listener;
	std::vector<cpp3ds::TcpSocket*> m_sockets;
	std::map<cpp3ds::TcpSocket*, Player> m_players;
	std::map<cpp3ds::TcpSocket*, bool> m_pingResponses;
	cpp3ds::SocketSelector m_selector;

	cpp3ds::Packet m_drawDataPacket;
};

} // namespace DrawAttack

#endif // DRAWATTACK_SERVER_HPP
