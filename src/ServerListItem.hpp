#ifndef DRAWATTACK_SERVERLISTITEM_HPP
#define DRAWATTACK_SERVERLISTITEM_HPP

#include <cpp3ds/Network.hpp>
#include "TweenTransformable.hpp"
#include "GUI/Button.hpp"

namespace DrawAttack {

class ServerListItem : public cpp3ds::Drawable, public util3ds::TweenTransformable {
public:
	ServerListItem(cpp3ds::IpAddress ip, unsigned short port);
	~ServerListItem();
	void ping(cpp3ds::Time timeout);
	void setActive(bool active);
	bool getActive() const;
	cpp3ds::IpAddress getIp() const;
	cpp3ds::FloatRect getRect() const;
	unsigned short getPort() const;
	unsigned short getPing() const;
protected:
	virtual void draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const;
private:
	static cpp3ds::Texture m_texture;
	cpp3ds::IpAddress m_ip;
	unsigned short m_port;
	unsigned short m_ping;
	gui3ds::Button m_button;
	bool m_active;
};

} // namespace DrawAttack

#endif // DRAWATTACK_SERVERLISTITEM_HPP
