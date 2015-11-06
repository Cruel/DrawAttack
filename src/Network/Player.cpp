#include "Player.hpp"

namespace DrawAttack {


Player::Player(std::string name)
: m_score(0)
, m_name(name)
{

}


Player::~Player()
{
	// Nothing
}


const std::string& Player::getName() const
{
	return m_name;
}


const unsigned int& Player::getScore() const
{
	return m_score;
}


void Player::setScore(const unsigned int &score)
{
	m_score = score;
}


void Player::incrementScore()
{
	m_score++;
}

} // namespace DrawAttack
