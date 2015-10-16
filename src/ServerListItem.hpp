#ifndef DRAWATTACK_SERVERLISTITEM_HPP
#define DRAWATTACK_SERVERLISTITEM_HPP

#include <cpp3ds/Network.hpp>
#include "TweenTransformable.hpp"

namespace DrawAttack {

class ServerListItem : public cpp3ds::Drawable, public util3ds::TweenTransformable {
friend class ServerList;
public:
	ServerListItem(cpp3ds::IpAddress ip, unsigned short port);
	~ServerListItem();
	void ping(cpp3ds::Time timeout);
	cpp3ds::IpAddress getIp() const;
	unsigned short getPort() const;
	unsigned short getPing() const;
protected:
	virtual void draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const;
private:
	cpp3ds::IpAddress m_ip;
	unsigned short m_port;
	unsigned short m_ping;
	cpp3ds::Text   m_text;
};

} // namespace DrawAttack

#endif // DRAWATTACK_SERVERLISTITEM_HPP
