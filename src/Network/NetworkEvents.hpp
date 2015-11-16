#ifndef DRAWATTACK_NETWORKEVENTS_HPP
#define DRAWATTACK_NETWORKEVENTS_HPP

#include <cpp3ds/Network/Packet.hpp>
#include <cpp3ds/System/String.hpp>
#include <cpp3ds/Graphics/Color.hpp>

namespace DrawAttack {

class NetworkEvent {
public:

	struct ServerEvent {
		std::string message;
	};

	struct ServerInfoEvent {
		std::string name;
		std::string language;
		unsigned int playerCount;
		unsigned int playerMax;
	};

	struct PlayerEvent {
		std::string name;
	};

	struct DrawEvent {
		int x, y;
	};

	struct WaitEvent {
		float value;
	};

	struct RoundEvent {
		std::string player;
		float time;
	};

	struct TextEvent {
		std::string name;
		cpp3ds::String value;
	};

	enum EventType {
		Version, // Must be first for backwards compatibility
		ServerShutdown,
		PlayerData,
		PlayerConnected,
		PlayerDisconnected,
		PlayerNameCollision,
		Text,
		DrawData,
		DrawMove,
		DrawEndline,
		DrawUndo,
		DrawClear,
		WaitForPlayers,
		RoundStart,
		RoundWord,
		RoundWin,
		RoundFail,
		RoundPass,
		RoundTimeout,
		Voice,
		Ping,

		ServerInfo,
		DrawColor,

		Count
	};

	EventType type;

//	union {
		PlayerEvent player;
		DrawEvent draw;
		TextEvent text;
		ServerEvent server;
		ServerInfoEvent serverInfo;
		WaitEvent wait;
		RoundEvent round;
		std::string roundWord;
		cpp3ds::Color color;
//	};

	struct Player {
		std::string name;
		unsigned int score;
	};

	std::vector<Player> playerData;

	static bool packetToEvent(cpp3ds::Packet& packet, NetworkEvent& event);
	static bool eventToPacket(NetworkEvent& event, cpp3ds::Packet& packet);
};

cpp3ds::Packet& operator <<(cpp3ds::Packet& packet, const NetworkEvent::EventType& type);
cpp3ds::Packet& operator >>(cpp3ds::Packet& packet, NetworkEvent::EventType& type);

cpp3ds::Packet& operator <<(cpp3ds::Packet& packet, const cpp3ds::Color& type);
cpp3ds::Packet& operator >>(cpp3ds::Packet& packet, cpp3ds::Color& type);

}

#endif // DRAWATTACK_NETWORKEVENTS_HPP
