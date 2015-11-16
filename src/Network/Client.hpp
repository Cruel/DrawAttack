#ifndef DRAWATTACK_CLIENT_HPP
#define DRAWATTACK_CLIENT_HPP

#include <cpp3ds/Network.hpp>
#include <cpp3ds/Graphics/Color.hpp>
#include "NetworkEvents.hpp"

#define CLIENT_VERSION "1.1"

namespace DrawAttack {

class Client {
public:
	Client();
	~Client();
	bool pollPacket(cpp3ds::Packet& packet);
	cpp3ds::Socket::Status connect(cpp3ds::IpAddress ip, unsigned short port);
	void disconnect();

	void flushPacket();
	void setRateLimit(const cpp3ds::Time& time);
	const cpp3ds::Time& getRateLimit() const;

	void sendText(std::string name, cpp3ds::String text);
	void sendPlayerConnected(cpp3ds::String name);
	void sendDrawMove(int x, int y);
	void sendDrawEndline(int x, int y);
	void sendUndo();
	void sendClear();
	void sendPing();
	void sendRoundPass();
	void sendColor(const cpp3ds::Color& color);
private:
	cpp3ds::TcpSocket m_socket;
	cpp3ds::Packet m_packet;
	cpp3ds::Clock m_rateClock;
	cpp3ds::Time m_rateLimit;
};

} // namespace DrawAttack

#endif // DRAWATTACK_CLIENT_HPP
