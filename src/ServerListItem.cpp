#include "ServerListItem.hpp"


namespace DrawAttack {


ServerListItem::ServerListItem(cpp3ds::IpAddress ip, unsigned short port)
{
	m_ip = ip;
	m_port = port;
	m_text.setString(m_ip.toString());
}


ServerListItem::~ServerListItem()
{

}


void ServerListItem::ping(cpp3ds::Time timeout)
{

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

	target.draw(m_text, states);
}


} // namespace DrawAttack
