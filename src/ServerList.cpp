#include <cpp3ds/System/FileSystem.hpp>
#include <TweenEngine/Tween.h>
#include <sstream>
#include "ServerList.hpp"
#include "Notification.hpp"

using namespace cpp3ds;
using namespace TweenEngine;

namespace DrawAttack {


ServerList::ServerList()
: m_selected(nullptr)
{
	Http http(SERVER_LIST_HOST);
	Http::Request request(SERVER_LIST_URI);
	Http::Response response = http.sendRequest(request);
	Http::Response::Status status = response.getStatus();
	if (status == Http::Response::Ok)
	{
		std::string body = response.getBody();
		std::istringstream stream(body);
		std::string line;
		while(std::getline(stream, line)) {
			char ip[256];
			int port;
			if (sscanf(line.c_str(), "%[^:]:%d", ip, &port) == 2) {
				addServer(ip, port);
			} else {
				err() << "Improper server ip format: " << line << std::endl;
			}
		}

		int i = 0;
		for(auto& item: m_servers) {
			item->move(-320.f, 0);
			item->ping(cpp3ds::seconds(2));
//			Tween::to(*item, ServerListItem::POSITION_X, 1.f)
//					.target(50)
//					.ease(TweenEquations::easeOutElastic)
//					.delay(1.f + 0.5f * i++)
//					.start(m_tweenManager);
		}
	}
	else
	{
		Notification::spawn(_("Failed to fetch server list."));
	}
}


void ServerList::reload()
{

}


void ServerList::ping(cpp3ds::Time timeout)
{

}


void ServerList::addServer(const char* ip, unsigned short port)
{
	std::unique_ptr<ServerListItem> item(new ServerListItem(ip, port));
	item->setPosition(0, m_servers.size() * 22.f);
	m_servers.emplace_back(std::move(item));
}


void ServerList::draw(cpp3ds::RenderTarget &target, cpp3ds::RenderStates states) const
{
	for(const auto& item: m_servers) {
		target.draw(*item, states);
	}
}


bool ServerList::processEvent(const cpp3ds::Event &event)
{
	if (event.type == Event::TouchEnded) {
		for(auto& item: m_servers) {
			FloatRect rect = item->getRect();
			rect.left += item->getPosition().x;
			rect.top += item->getPosition().y;
			if (rect.contains(event.touch.x, event.touch.y)) {
				if (m_selected)
					m_selected->setActive(false);
				m_selected = item.get();
				item->setActive(true);
				return false;
			}
		}
	}

	return true;
}


void ServerList::update(float delta) {
	m_tweenManager.update(delta);
	for(auto& item: m_servers) {
		item->update(delta);
	}
}


ServerListItem* ServerList::getSelectedItem() {
	return m_selected;
}


void ServerList::deselect() {
	m_selected = nullptr;
	for(auto& item: m_servers) {
		item->setActive(false);
	}
}


bool ServerList::isLoading() const
{
	for(auto& item: m_servers) {
		if (item->isPinging())
			return true;
	}
	return false;
}

} // namespace DrawAttack
