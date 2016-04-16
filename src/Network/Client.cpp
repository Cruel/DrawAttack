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
	cpp3ds::Socket::Status status = m_socket.connect(ip, port);
	if (status == cpp3ds::Socket::Done) {
		m_packet << NetworkEvent::Version << std::string(CLIENT_VERSION);
	} else {
		m_socket.disconnect();
	}
	m_socket.setBlocking(false);
	return status;
}


void Client::disconnect()
{
	m_socket.disconnect();
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
}


void Client::sendDrawMove(int x, int y)
{
	m_packet << NetworkEvent::DrawMove << x << y;
}


void Client::sendDrawEndline(int x, int y)
{
	m_packet << NetworkEvent::DrawEndline << x << y;
}


void Client::sendPlayerConnected(cpp3ds::String name)
{
	m_packet << NetworkEvent::PlayerConnected << name.toAnsiString();
}


void Client::sendUndo()
{
	m_packet << NetworkEvent::DrawUndo;
}


void Client::sendClear()
{
	m_packet << NetworkEvent::DrawClear;
}


void Client::sendPing()
{
	m_packet << NetworkEvent::Ping;
}


void Client::sendRoundPass()
{
	m_packet << NetworkEvent::RoundPass;
}


void Client::sendColor(const cpp3ds::Color &color)
{
	m_packet << NetworkEvent::DrawColor << color;
}


void Client::setRateLimit(const cpp3ds::Time &time)
{
	m_rateLimit = time;
}


const cpp3ds::Time& Client::getRateLimit() const
{
	return m_rateLimit;
}


void Client::sendVoiceData(const std::string& name, const char *samples, unsigned int sizeInBytes)
{
	m_packet << NetworkEvent::VoiceData << name << sizeInBytes;
	m_packet.append(samples, sizeInBytes);
}


void Client::sendVoiceEnd(const std::string& name)
{
	m_packet << NetworkEvent::VoiceEnd << name << static_cast<unsigned int>(0);
}

} // namespace DrawAttack
