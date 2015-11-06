#include "Client.hpp"


namespace DrawAttack {


Client::Client()
: m_rateLimit(cpp3ds::milliseconds(50))
{

}


Client::~Client()
{

}


cpp3ds::Socket::Status Client::connect(cpp3ds::IpAddress ip, unsigned short port)
{
	m_socket.setBlocking(true);
	cpp3ds::Socket::Status status = m_socket.connect(ip, port, cpp3ds::seconds(1.f));
	if (status == cpp3ds::Socket::Done) {
		m_socket.setBlocking(false);
	} else {
		m_socket.disconnect();
	}
	return status;
}


bool Client::pollPacket(cpp3ds::Packet& packet)
{
	cpp3ds::Socket::Status status = m_socket.receive(packet);
	if (status == cpp3ds::Socket::Done) {
		return true;
	}
	if (status == cpp3ds::Socket::Disconnected || status == cpp3ds::Socket::Error) {
		m_socket.disconnect();
		return false;
	}
	return true;
}


void Client::flushPacket()
{
	if (!m_packet.endOfPacket() && m_rateClock.getElapsedTime() > m_rateLimit) {
		m_socket.send(m_packet);
		m_packet.clear();
		m_rateClock.restart();
	}
}


void Client::sendText(std::string name, cpp3ds::String text)
{
	m_packet << NetworkEvent::Text << name << text;
	flushPacket();
}


void Client::sendDrawMove(int x, int y)
{
	m_packet << NetworkEvent::DrawMove << x << y;
	flushPacket();
}


void Client::sendDrawEndline(int x, int y)
{
	m_packet << NetworkEvent::DrawEndline << x << y;
	flushPacket();
}


void Client::sendPlayerConnected(cpp3ds::String name)
{
	m_packet << NetworkEvent::PlayerConnected << name.toAnsiString();
	flushPacket();
}


void Client::sendUndo()
{
	m_packet << NetworkEvent::DrawUndo;
	flushPacket();
}


void Client::sendClear()
{
	m_packet << NetworkEvent::DrawClear;
	flushPacket();
}


void Client::sendPing()
{
	m_packet << NetworkEvent::Ping;
	flushPacket();
}


void Client::setRateLimit(const cpp3ds::Time &time)
{
	m_rateLimit = time;
}


const cpp3ds::Time& Client::getRateLimit() const
{
	return m_rateLimit;
}

} // namespace DrawAttack
