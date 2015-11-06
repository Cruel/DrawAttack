#include "ServerListItem.hpp"


namespace DrawAttack {


cpp3ds::Texture ServerListItem::m_texture;


ServerListItem::ServerListItem(cpp3ds::IpAddress ip, unsigned short port)
{
	if (m_texture.getSize().x == 0)
		m_texture.loadFromFile("images/button-radius.9.png");
	m_button.setTexture(&m_texture);
	m_button.setString(ip.toString());
	m_ip = ip;
	m_port = port;
	setActive(false);
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

} // namespace DrawAttack
