#ifndef DRAWATTACK_PLAYER_HPP
#define DRAWATTACK_PLAYER_HPP

#include <cpp3ds/Network.hpp>

namespace DrawAttack {

class Player {
public:
	Player(cpp3ds::TcpSocket* socket, std::string name);
	~Player();

	std::string getName();
	cpp3ds::TcpSocket* socket;
private:
	std::string m_name;
};

} // namespace DrawAttack

#endif //DRAWATTACK_PLAYER_HPP
