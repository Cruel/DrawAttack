#include "Player.hpp"

namespace DrawAttack {


Player::Player(cpp3ds::TcpSocket *socket, std::string name)
: socket(socket)
, m_name(name)
{

}


Player::~Player() {
//	delete socket;
}


std::string Player::getName() {
	return m_name;
}


} // namespace DrawAttack
