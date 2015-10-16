#include "Client.hpp"


namespace DrawAttack {


Client::Client()
{

}


Client::~Client()
{

}


cpp3ds::Socket::Status Client::connect(cpp3ds::IpAddress ip, unsigned short port)
{
	cpp3ds::Socket::Status status = m_socket.connect(ip, port, cpp3ds::seconds(2.f));
	if (status == cpp3ds::Socket::Done) {
		m_socket.setBlocking(false);
	} else {
		std::cout << "connection failed" << std::endl;
	}
	return status;
}


bool Client::pollEvent(NetworkEvent &event)
{
	cpp3ds::Packet packet;
	cpp3ds::Socket::Status status = m_socket.receive(packet);
	if (status == cpp3ds::Socket::Done) {
		cpp3ds::Uint8 type;
		packet >> type;
		event.type = static_cast<NetworkEvent::EventType>(type);
		switch (type) {
			case NetworkEvent::ServerShutdown:
				packet >> event.server.message;
				return true;
			case NetworkEvent::PlayerData:
				packet >> event.playerData.count;
				for (int i = 0; i < event.playerData.count; i++) {
					std::string name;
					packet >> name;
					event.playerData.players.push_back(name);
				}
				return true;
			case NetworkEvent::PlayerConnected:
			case NetworkEvent::PlayerDisconnected:
				packet >> event.player.name;
				return true;
			case NetworkEvent::Text:
				packet >> event.text.value;
				std::cout << event.text.value.toAnsiString() << std::endl;
				return true;
			case NetworkEvent::DrawMove:
			case NetworkEvent::DrawEndline:
				packet >> event.draw.x >> event.draw.y;
				return true;
			case NetworkEvent::DrawerDesignation:
				packet >> event.player.name;
				return true;
			default:
				cpp3ds::err() << "Unknown event received: " << event.type << std::endl;
				return false;
		}
	}
	if (status == cpp3ds::Socket::Disconnected) {
		event.type = NetworkEvent::ServerShutdown;
		event.server.message = "(None)";
		return true;
	}
	return false;
}


void Client::sendText(cpp3ds::String text)
{
	cpp3ds::Packet packet;
	packet << static_cast<cpp3ds::Uint8>(NetworkEvent::Text);
	packet << text;
	m_socket.send(packet);
}


void Client::sendDrawMove(int x, int y)
{
	cpp3ds::Packet packet;
	packet << static_cast<cpp3ds::Uint8>(NetworkEvent::DrawMove);
	packet << x << y;
	m_socket.send(packet);
}


void Client::sendDrawEndline(int x, int y)
{
	cpp3ds::Packet packet;
	packet << static_cast<cpp3ds::Uint8>(NetworkEvent::DrawEndline);
	packet << x << y;
	m_socket.send(packet);
}


void Client::sendPlayerConnected(cpp3ds::String name)
{
	cpp3ds::Packet packet;
	packet << static_cast<cpp3ds::Uint8>(NetworkEvent::PlayerConnected);
	packet << name.toAnsiString();
	m_socket.send(packet);
}


} // namespace DrawAttack
