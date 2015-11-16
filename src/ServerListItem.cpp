#include <TweenEngine/Tween.h>
#include "ServerListItem.hpp"
#include "Network/NetworkEvents.hpp"


namespace DrawAttack {


cpp3ds::Texture ServerListItem::m_texture;


ServerListItem::ServerListItem(const char* ip, unsigned short port)
: m_thread(&ServerListItem::threadFunc, this)
, m_pinging(false)
{
	if (m_texture.getSize().x == 0)
		m_texture.loadFromFile("images/button-radius.9.png");
	m_button.setTexture(&m_texture);
	m_button.setString(_("%s:%d", ip, port));
	m_button.getText().setCharacterSize(14);
	m_ip = ip;
	m_port = port;
	setActive(false);
}


ServerListItem::~ServerListItem()
{

}


void ServerListItem::ping(cpp3ds::Time timeout)
{
	m_pingTimeout = timeout;
	m_thread.launch();
}


cpp3ds::IpAddress ServerListItem::getIp() const
{
	return m_ip;
}


unsigned short ServerListItem::getPort() const
{
	return m_port;
}


unsigned short ServerListItem::getPing() const
{
	return m_ping;
}


void ServerListItem::draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_button, states);
}


void ServerListItem::setActive(bool active)
{
	m_active = active;
	m_button.setColor(active ? cpp3ds::Color(0, 0, 200, 100) : cpp3ds::Color::Transparent);
}


bool ServerListItem::getActive() const
{
	return m_active;
}


cpp3ds::FloatRect ServerListItem::getRect() const
{
	return cpp3ds::FloatRect(m_button.getPosition().x, m_button.getPosition().y, m_button.getSize().x, m_button.getSize().y);
}


void ServerListItem::threadFunc()
{
	cpp3ds::Clock clock;
	cpp3ds::TcpSocket socket;
	NetworkEvent event;

	m_pinging = true;

	socket.setBlocking(true);
	cpp3ds::Socket::Status status = socket.connect(m_ip, m_port);

	if (status != cpp3ds::Socket::Done)
	{
		m_button.setTextColor(cpp3ds::Color(200, 0, 0, 150));
	}
	else
	{
		socket.setBlocking(false);
		cpp3ds::Packet packet;
		packet << NetworkEvent::ServerInfo;
		status = socket.send(packet);

		while (status != cpp3ds::Socket::Done) {
			if (status != cpp3ds::Socket::Partial) {
				cpp3ds::err() << "Failed to send packet: " << m_ip.toString() << std::endl;
				return;
			}
			cpp3ds::sleep(cpp3ds::milliseconds(20));
			status = socket.send(packet);
		}

		event.type = NetworkEvent::Count;
		clock.restart();
		packet.clear();

		while (event.type != NetworkEvent::ServerInfo && clock.getElapsedTime() <= m_pingTimeout) {
			while (!NetworkEvent::packetToEvent(packet, event) && clock.getElapsedTime() <= m_pingTimeout) {
				status = socket.receive(packet);
				while (status != cpp3ds::Socket::Done && clock.getElapsedTime() <= m_pingTimeout) {
					cpp3ds::sleep(cpp3ds::milliseconds(20));
					status = socket.receive(packet);
				}
			}
		}

		socket.disconnect();

		if (clock.getElapsedTime() > m_pingTimeout) {
			cpp3ds::err() << "Timed out pinging " << m_ip << ":" << m_port << std::endl;
			m_button.setTextColor(cpp3ds::Color(200, 0, 0, 150));
		} else {
			m_button.setTextColor(cpp3ds::Color::Black);
			m_button.setString(_("%d/%d  %s", event.serverInfo.playerCount, event.serverInfo.playerMax,
			                     event.serverInfo.name.c_str()));
		}

	}

	m_pinging = false;

	TweenEngine::Tween::to(*this, POSITION_X, 1.f)
			.target(20)
			.ease(TweenEngine::TweenEquations::easeOutElastic)
			.start(m_tweenManager);
}


void ServerListItem::update(float delta)
{
	m_tweenManager.update(delta);
}


bool ServerListItem::isPinging() const
{
	return m_pinging;
}

} // namespace DrawAttack
