#ifndef DRAWATTACK_SERVERCONFIG_HPP
#define DRAWATTACK_SERVERCONFIG_HPP

#include "../Keyboard/tinyxml2.h"
#include <string>

namespace DrawAttack {

class ServerConfig {
public:
	ServerConfig(const std::string& filename);

	const std::string& getServerName() const;
	const std::string& getLanguage() const;
	const float getMinPlayers() const;
	const float getMaxPlayers() const;
	const float getRoundDuration() const;
	const float getRoundIntermission() const;
	const float getInactiveTimeout() const;

private:
	std::string m_name;
	std::string m_language;

	float m_minPlayers;
	float m_maxPlayers;
	float m_roundDuration;
	float m_roundIntermission;
	float m_inactiveTimeout;
};

} // namespace DrawAttack

#endif // DRAWATTACK_SERVERCONFIG_HPP
