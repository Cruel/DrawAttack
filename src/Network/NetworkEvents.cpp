#include "NetworkEvents.hpp"

namespace DrawAttack {

cpp3ds::Packet& operator <<(cpp3ds::Packet& packet, const NetworkEvent::EventType& type)
{
	return packet << static_cast<cpp3ds::Uint8>(type);
}

cpp3ds::Packet& operator >>(cpp3ds::Packet& packet, NetworkEvent::EventType& type)
{
	cpp3ds::Uint8 val;
	packet >> val;
	type = static_cast<NetworkEvent::EventType>(val);
	return packet;
}

cpp3ds::Packet& operator <<(cpp3ds::Packet& packet, const cpp3ds::Color& color)
{
	return packet << color.r << color.g << color.b;
}

cpp3ds::Packet& operator >>(cpp3ds::Packet& packet, cpp3ds::Color& color)
{
	color.a = 255;
	packet >> color.r >> color.g >> color.b;
	return packet;
}

bool NetworkEvent::packetToEvent(cpp3ds::Packet& packet, NetworkEvent& event)
{
	if (packet.endOfPacket())
		return false;
	packet >> event.type;
	switch (event.type) {
		case NetworkEvent::Version:
		case NetworkEvent::ServerShutdown:
			packet >> event.server.message;
			break;
		case NetworkEvent::ServerInfo:
			packet >> event.serverInfo.name >> event.serverInfo.language;
			packet >> event.serverInfo.playerCount >> event.serverInfo.playerMax;
			break;
		case NetworkEvent::PlayerData: {
			cpp3ds::Uint8 playerCount;
			packet >> playerCount;
			for (int i = 0; i < playerCount; i++) {
				NetworkEvent::Player player;
				packet >> player.name >> player.score;
				event.playerData.push_back(player);
			}
			break;
		}
		case NetworkEvent::PlayerConnected:
		case NetworkEvent::PlayerDisconnected:
		case NetworkEvent::PlayerNameCollision:
			packet >> event.player.name;
			break;
		case NetworkEvent::Text:
			packet >> event.text.name >> event.text.value;
			break;
		case NetworkEvent::DrawMove:
		case NetworkEvent::DrawEndline:
			packet >> event.draw.x >> event.draw.y;
			break;
		case NetworkEvent::DrawColor:
			packet >> event.color;
			break;
		case NetworkEvent::WaitForPlayers:
			packet >> event.wait.value;
			break;
		case NetworkEvent::RoundWord:
			packet >> event.roundWord;
			break;
		case NetworkEvent::RoundStart:
		case NetworkEvent::RoundWin:
			packet >> event.round.player >> event.round.time;
			break;
		case NetworkEvent::DrawData:
		case NetworkEvent::DrawUndo:
		case NetworkEvent::DrawClear:
		case NetworkEvent::RoundFail:
		case NetworkEvent::RoundPass:
		case NetworkEvent::RoundTimeout:
		case NetworkEvent::Ping:
			break;
		default:
			return false;
	}
	return true;
}


bool NetworkEvent::eventToPacket(NetworkEvent &event, cpp3ds::Packet &packet)
{
	packet << event.type;
	switch (event.type) {
		case NetworkEvent::Version:
		case NetworkEvent::ServerShutdown:
			packet << event.server.message;
			break;
		case NetworkEvent::ServerInfo:
			packet << event.serverInfo.name << event.serverInfo.language;
			packet << event.serverInfo.playerCount << event.serverInfo.playerMax;
			break;
		case NetworkEvent::PlayerData:
			break;
		case NetworkEvent::PlayerConnected:
		case NetworkEvent::PlayerDisconnected:
		case NetworkEvent::PlayerNameCollision:
			packet << event.player.name;
			break;
		case NetworkEvent::Text:
			packet << event.text.name << event.text.value;
			break;
		case NetworkEvent::DrawMove:
		case NetworkEvent::DrawEndline:
			packet << event.draw.x << event.draw.y;
			break;
		case NetworkEvent::DrawColor:
			packet << event.color;
			break;
		case NetworkEvent::WaitForPlayers:
			packet << event.wait.value;
			break;
		case NetworkEvent::RoundWord:
			packet << event.roundWord;
			break;
		case NetworkEvent::RoundStart:
		case NetworkEvent::RoundWin:
			packet << event.round.player >> event.round.time;
			break;
		case NetworkEvent::DrawData:
		case NetworkEvent::DrawUndo:
		case NetworkEvent::DrawClear:
		case NetworkEvent::RoundFail:
		case NetworkEvent::RoundPass:
		case NetworkEvent::RoundTimeout:
		case NetworkEvent::Ping:
			break;
		default:
			return true;
	}
	return true;
}

} // namespace DrawAttack
