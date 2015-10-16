#ifndef DRAWATTACK_NETWORKEVENTS_HPP
#define DRAWATTACK_NETWORKEVENTS_HPP

namespace DrawAttack {

class NetworkEvent {
public:

//	NetworkEvent() : text() {}

	struct ServerEvent {
		std::string message;
	};

	struct PlayerDataEvent {
		cpp3ds::Uint8 count;
		std::vector<std::string> players;
	};

	struct PlayerEvent {
		std::string name;
	};

	struct DrawEvent {
		int x, y;
	};

	struct TextEvent {
		cpp3ds::String value;
	};

	enum EventType {
		ServerShutdown,
		PlayerData,
		PlayerConnected,
		PlayerDisconnected,
		Text,
		DrawMove,
		DrawEndline,
		DrawUndo,
		DrawerDesignation,
		Voice,

		Count
	};

	EventType type;

//	union {
		PlayerDataEvent playerData;
		PlayerEvent player;
		DrawEvent draw;
		TextEvent text;
		ServerEvent server;
//	};
};

}

#endif //DRAWATTACK_NETWORKEVENTS_HPP
