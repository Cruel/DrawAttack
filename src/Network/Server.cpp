#include <fstream>
#include "Server.hpp"
#include "NetworkEvents.hpp"


namespace DrawAttack {


Server::Server(unsigned short port, std::string wordFilename)
: m_port(port)
, m_running(false)
, m_wordFilename(wordFilename)
{
	std::ifstream wordFile(wordFilename);
	std::string line;
	if (wordFile.is_open()) {
		while (std::getline(wordFile, line)) {
			m_wordList.push_back(line);
		}
		wordFile.close();
	} else {
		cpp3ds::err() << "Failed to open word file: " << wordFilename << std::endl;
	}
}


Server::~Server() {
	for (auto& socket : m_sockets)
		delete socket;
}


void Server::run() {
	if (m_wordList.size() == 0)
		return;

	if (m_listener.listen(m_port) == cpp3ds::Socket::Done) {
		m_selector.add(m_listener);
		std::cout << "Started DrawAttack server on port " << m_port << "..." << std::endl;
		m_running = true;
		while (m_running) {
			// Make the selector wait for data on any socket
			if (m_selector.wait(cpp3ds::milliseconds(500))) {
				if (m_selector.isReady(m_listener)) {
					cpp3ds::TcpSocket* socket = new cpp3ds::TcpSocket;
					if (m_listener.accept(*socket) == cpp3ds::Socket::Done)
					{
						std::cout << "client connected" << std::endl;
						// Add the new client to the clients list
						m_sockets.push_back(socket);
						m_selector.add(*socket);
						sendPlayerData(socket);
					} else {
						delete socket;
					}
				}
				else
				{
					for (auto& socket : m_sockets) {
						if (m_selector.isReady(*socket)) {
							processSocket(socket);
						}
					}
				}
			}
		}
	} else {
		cpp3ds::err() << "Failed to listen on port " << m_port << std::endl;
	}
}


void Server::exit()
{
	m_running = false;
}


void Server::sendToAll(cpp3ds::Packet &packet)
{
	for (auto& player : m_players) {
		player.socket->send(packet);
	}
}


void Server::sendPlayerData(cpp3ds::TcpSocket* socket) {
	cpp3ds::Packet packet;
	packet << static_cast<cpp3ds::Uint8>(NetworkEvent::EventType::PlayerData);
	packet << static_cast<cpp3ds::Uint8>(m_players.size());
	for (auto& p : m_players) {
		packet << p.getName();
	}
	socket->send(packet);
}


void Server::processSocket(cpp3ds::TcpSocket* socket)
{
	cpp3ds::Packet packet;
	cpp3ds::Socket::Status status = socket->receive(packet);
	if (status == cpp3ds::Socket::Done)
	{
		cpp3ds::Packet packetSend = packet;
		cpp3ds::Uint8 typeInt;
		packet >> typeInt;
		NetworkEvent::EventType type = static_cast<NetworkEvent::EventType>(typeInt);

		switch(type) {
			case NetworkEvent::PlayerConnected: {
				std::string name;
				packet >> name;
				std::cout << name << " connected." << std::endl;
				Player player(socket, name);
				m_players.push_back(player);
				sendToAll(packetSend);
				sendDrawerDesignation(player);
				break;
			}
			case NetworkEvent::Text: {
				cpp3ds::String text;
				packet >> text;
				std::cout << "message: " << text.toAnsiString() << std::endl;
				sendToAll(packetSend);
				break;
			}
			case NetworkEvent::DrawMove:
			case NetworkEvent::DrawEndline: {
				sendToAll(packetSend);
				break;
			}
			default:
				break;
		}
	} else if (status == cpp3ds::Socket::Disconnected) {
		std::cout << "Player disconnected!" << std::endl;
		m_selector.remove(*socket);
		for (std::vector<Player>::iterator i = m_players.begin(); i != m_players.end(); i++)
			if (i->socket == socket) {
				m_players.erase(i);
				break;
			}
		for (std::vector<cpp3ds::TcpSocket*>::iterator i = m_sockets.begin(); i != m_sockets.end(); i++)
			if (*i == socket) {
				m_sockets.erase(i);
				break;
			}
		delete socket;
	}
}


void Server::sendDrawerDesignation(Player &drawer)
{
	cpp3ds::Packet packet;
	packet << static_cast<cpp3ds::Uint8>(NetworkEvent::DrawerDesignation);
	packet << drawer.getName();
	sendToAll(packet);
}


} // namespace DrawAttack
