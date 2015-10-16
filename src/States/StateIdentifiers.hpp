#ifndef DRAWATTACK_STATEIDENTIFIERS_HPP
#define DRAWATTACK_STATEIDENTIFIERS_HPP

namespace DrawAttack {
	namespace States {
		enum ID {
			None,
			Title,
			Menu,
			ServerSelect,
			NameSelect,
			Play,
			Loading,
			Pause,
			Disconnected,
		};
	}
}

#endif // DRAWATTACK_STATEIDENTIFIERS_HPP
