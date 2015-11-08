#include <fstream>
#include "Server.hpp"
#include "NetworkEvents.hpp"


namespace DrawAttack {


Server::Server(unsigned short port, std::string wordFilename)
: m_port(port)
, m_running(false)
, m_wordFilename(wordFilename)
, m_mode(Wait)
, m_currentDrawer(nullptr)
{
	std::ifstream wordFile(wordFilename);
	std::string line;
	if (wordFile.is_open()) {
		while (std::getline(wordFile, line)) {
			std::transform(line.begin(), line.end(), line.begin(), ::tolower);
			m_wordList.push_back(line);
		}
		wordFile.close();
	} else {
		cpp3ds::err() << "Failed to open word file: " << wordFilename << std::endl;
	}

	clearDrawData();
}


Server::~Server() {
	for (auto& socket : m_sockets) {
		socket->disconnect();
		delete socket;
	}
	m_listener.close();
}


void Server::run() {
	if (m_wordList.size() == 0)
		return;

	cpp3ds::Clock pingClock;

	if (m_listener.listen(m_port) == cpp3ds::Socket::Done) {
		m_selector.add(m_listener);
		std::cout << "Started DrawAttack server on port " << m_port << "..." << std::endl;
		m_running = true;
		while (m_running) {
			// Make the selector wait for data on any socket
			if (m_selector.wait(cpp3ds::milliseconds(1000))) {
				if (m_selector.isReady(m_listener)) {
					cpp3ds::TcpSocket* socket = new cpp3ds::TcpSocket;
					if (m_listener.accept(*socket) == cpp3ds::Socket::Done)
					{
						std::cout << "client connected" << std::endl;
						// Add the new client to the clients list
						sendPlayerData(socket);
						socket->send(m_drawDataPacket);
						m_pingResponses[socket] = true;
						m_sockets.emplace_back(socket);
						m_selector.add(*socket);
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

			// Ping clients and check for previous response
			if (pingClock.getElapsedTime() >= cpp3ds::seconds(PING_TIMEOUT)) {
				cpp3ds::Packet packet;
				packet << NetworkEvent::Ping;
				for (auto i = m_sockets.begin(); i != m_sockets.end();) {
					if (m_pingResponses[*i]) {
						(*i)->send(packet);
						m_pingResponses[*i] = false;
						i++;
					} else {
						// Timed out socket
						std::cout << "A socket timed out." << std::endl;
						removeSocket(*i);
					}
				}
				pingClock.restart();
			}

			if (m_mode == Play) {
				if (m_roundClock.getElapsedTime() >= cpp3ds::seconds(ROUND_DURATION)) {
					// Round time ended, nobody won
					cpp3ds::Packet packet;
					packet << NetworkEvent::RoundWord << m_currentWord << NetworkEvent::RoundFail;
					sendToAllSockets(packet);
					m_roundClock.restart();
					m_mode = Wait;
				}
				if (m_roundTimeoutClock.getElapsedTime() >= cpp3ds::seconds(ROUND_TIMEOUT)) {
					// Drawer hasn't been active, so end round
					cpp3ds::Packet packet;
					packet << NetworkEvent::RoundWord << m_currentWord << NetworkEvent::RoundTimeout;
					sendToAllSockets(packet);
					m_roundClock.restart();
					m_mode = Wait;
				}
			} else if (m_mode == Wait) {
				if (m_players.size() >= MIN_PLAYERS) {
					if (m_roundClock.getElapsedTime() >= cpp3ds::seconds(ROUND_INTERMISSION)) {
						Player drawer = getNextDrawer();
						startRound(drawer, getNextWord(), ROUND_DURATION);
					}
				}
			}
		}
	} else {
		cpp3ds::err() << "Failed to listen on port " << m_port << std::endl;
	}
}


void Server::exit(std::string reason)
{
	cpp3ds::Packet packet;
	if (reason.empty())
		reason = "Shutdown by admin";
	packet << NetworkEvent::ServerShutdown << reason;
	sendToAllSockets(packet);
	m_running = false;
}


void Server::sendToAllSockets(cpp3ds::Packet &packet)
{
	for (auto& socket : m_sockets) {
		socket->send(packet);
	}
}


void Server::sendToAllPlayers(cpp3ds::Packet &packet)
{
	for (auto& player : m_players) {
		player.first->send(packet);
	}
}


void Server::sendPlayerData(cpp3ds::TcpSocket* socket) {
	cpp3ds::Packet packet;
	packet << NetworkEvent::PlayerData;
	packet << static_cast<cpp3ds::Uint8>(m_players.size());
	for (const auto& player : m_players) {
		packet << player.second.getName();
		packet << player.second.getScore();
	}
	socket->send(packet);
}


void Server::processSocket(cpp3ds::TcpSocket* socket)
{
	cpp3ds::Packet packet;
	cpp3ds::Socket::Status status = socket->receive(packet);
	if (status == cpp3ds::Socket::Done)
	{
		cpp3ds::Packet packetSend;
		NetworkEvent event;

		while (NetworkEvent::packetToEvent(packet, event))
		{
			if (!validateEvent(socket, event))
				continue;

			switch(event.type) {
				case NetworkEvent::Version:
					if (event.server.message.compare(SERVER_VERSION) != 0) {
						packet.clear();
						std::string message = _("Incompatible client version %s (needs %s)", event.server.message.c_str(), SERVER_VERSION);
						packet << NetworkEvent::ServerShutdown << message;
						socket->send(packet);
						removeSocket(socket);
					}
					break;
				case NetworkEvent::PlayerConnected: {
					std::cout << event.player.name << " connected." << std::endl;
					bool collision = false;
					for (auto& player : m_players) {
						if (event.player.name.compare(player.second.getName()) == 0) {
							collision = true;
							break;
						}
					}
					if (collision) {
						cpp3ds::Packet packet;
						packet << NetworkEvent::PlayerNameCollision << event.player.name;
						socket->send(packet);
						break;
					}
					Player player(event.player.name);
					m_players.emplace(socket, player);
					NetworkEvent::eventToPacket(event, packetSend);
					sendToAllSockets(packetSend);
					packetSend.clear();
					if (m_players.size() < MIN_PLAYERS) {
						sendWaitForPlayers(socket, MIN_PLAYERS);
					}
					break;
				}
				case NetworkEvent::Text: {
					NetworkEvent::eventToPacket(event, packetSend);
					if (m_mode != Play)
						break;
					std::string word = event.text.value.toAnsiString();
					std::transform(word.begin(), word.end(), word.begin(), ::tolower);
					if (word.compare(m_currentWord) == 0) {
						std::cout << event.text.name << " won!" << std::endl;
						packetSend << NetworkEvent::RoundWord << word << NetworkEvent::RoundWin << event.text.name;
						for (auto& player : m_players) {
							if (player.second.getName().compare(event.text.name) == 0)
								player.second.incrementScore();
							if (player.first == m_currentDrawer)
								player.second.incrementScore();
						}
						m_roundClock.restart();
						m_mode = Wait;
					}
					break;
				}
				case NetworkEvent::DrawMove:
				case NetworkEvent::DrawEndline:
					m_drawDataPacket << event.type << event.draw.x << event.draw.y;
					NetworkEvent::eventToPacket(event, packetSend);
					m_roundTimeoutClock.restart();
					break;
				case NetworkEvent::DrawClear:
					clearDrawData();
					NetworkEvent::eventToPacket(event, packetSend);
					m_roundTimeoutClock.restart();
					break;
				case NetworkEvent::DrawUndo: {
					m_drawDataPacket << event.type;
					NetworkEvent::eventToPacket(event, packetSend);
					m_roundTimeoutClock.restart();
					break;
				}
				case NetworkEvent::RoundPass:
					packetSend << NetworkEvent::RoundWord << m_currentWord;
					NetworkEvent::eventToPacket(event, packetSend);
					m_roundClock.restart();
					m_mode = Wait;
					break;
				case NetworkEvent::Ping:
					m_pingResponses[socket] = true;
					break;
				default:
					break;
			}
		}
		if (!packetSend.endOfPacket())
			sendToAllSockets(packetSend);

	} else if (status == cpp3ds::Socket::Disconnected || status == cpp3ds::Socket::Error) {
		removeSocket(socket);
	}
}


bool Server::validateEvent(cpp3ds::TcpSocket* socket, const NetworkEvent &event)
{
	switch (event.type) {
		case NetworkEvent::PlayerConnected:
			break;
		case NetworkEvent::RoundPass:
		case NetworkEvent::DrawMove:
		case NetworkEvent::DrawEndline:
		case NetworkEvent::DrawUndo:
		case NetworkEvent::DrawClear:
			if (m_currentDrawer != socket || m_mode == Wait)
				return false;
			break;
		default:
			break;
	}

	// Assume it's good if none of the above checks failed
	return true;
}


void Server::startRound(Player &drawer, std::string word, float duration)
{
	std::cout << "Starting round. Drawer: " << drawer.getName() << " Word: " << word << std::endl;
	m_roundDuration = duration;
	m_currentWord = word;
	m_mode = Play;

	cpp3ds::Packet packet;
	packet << NetworkEvent::RoundStart << drawer.getName() << duration;
	sendToAllSockets(packet);

	// Send word to the drawer
	packet.clear();
	packet << NetworkEvent::RoundWord << word;
	m_currentDrawer->send(packet);

	m_roundClock.restart();
	m_roundTimeoutClock.restart();
	clearDrawData();
}


void Server::sendDrawData(cpp3ds::TcpSocket *socket)
{

}


void Server::clearDrawData()
{
	m_drawDataPacket.clear();
	m_drawDataPacket << NetworkEvent::DrawData;
}


void Server::removeSocket(cpp3ds::TcpSocket *socket)
{
	cpp3ds::Packet packet;
	std::string name;
	m_selector.remove(*socket);
	for (auto i = m_players.begin(); i != m_players.end(); i++)
		if (i->first == socket) {
			name = i->second.getName();
			m_players.erase(i);
			break;
		}
	for (auto i = m_sockets.begin(); i != m_sockets.end(); i++)
		if (*i == socket) {
			m_sockets.erase(i);
			break;
		}
	delete socket;

	// If name is empty, it was just an idle (spectating) socket
	if (!name.empty()) {
		std::cout << "Player disconnected: " << name << std::endl;
		packet << NetworkEvent::PlayerDisconnected << name;
		if (m_players.size() < MIN_PLAYERS) {
			m_mode = Wait;
			packet << NetworkEvent::WaitForPlayers << MIN_PLAYERS;
		}
		sendToAllSockets(packet);
	}
}


void Server::sendWaitForPlayers(cpp3ds::TcpSocket *socket, float playersNeeded)
{
	cpp3ds::Packet packet;
	packet << NetworkEvent::WaitForPlayers << playersNeeded;
	socket->send(packet);
}


const Player& Server::getNextDrawer()
{
	if (m_currentDrawer) {
		bool found = false;
		for (auto& player : m_players) {
			if (found) {
				m_currentDrawer = player.first;
				return player.second;
			}
			if (player.first == m_currentDrawer)
				found = true;
		}
	}
	m_currentDrawer = m_players.begin()->first;
	return m_players.begin()->second;
}


std::string Server::getNextWord()
{
	if (m_wordList.empty()) {
		if (m_wordListUsed.empty())
			return "Error: No word list";
		m_wordList = m_wordListUsed;
		m_wordListUsed.clear();
	}
	auto iterator = m_wordList.begin();
	std::advance(iterator, std::rand() % m_wordList.size());
	std::string nextWord = *iterator;

	m_wordListUsed.push_back(nextWord);
	m_wordList.erase(iterator);

	return nextWord;
}

} // namespace DrawAttack
