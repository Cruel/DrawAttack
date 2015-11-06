#ifndef DRAWATTACK_PLAYER_HPP
#define DRAWATTACK_PLAYER_HPP

#include <cpp3ds/Network.hpp>

namespace DrawAttack {

class Player {
public:
	Player(std::string name);
	~Player();

	const std::string& getName() const;

	void incrementScore();
	void setScore(const unsigned int& score);
	const unsigned int& getScore() const;
private:
	std::string m_name;
	unsigned int m_score;
};

} // namespace DrawAttack

#endif // DRAWATTACK_PLAYER_HPP
