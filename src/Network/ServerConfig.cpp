#include <cpp3ds/System/Err.hpp>
#include "ServerConfig.hpp"

namespace DrawAttack {

ServerConfig::ServerConfig(const std::string &filename)
: m_name("DrawAttack Server")
, m_minPlayers(2.f)
, m_maxPlayers(16.f)
, m_roundDuration(60.f)
, m_roundIntermission(8.f)
, m_inactiveTimeout(15.f)
{
	tinyxml2::XMLDocument xml;
	if (xml.LoadFile(filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement* server = xml.FirstChildElement("server");
		if (server != NULL)
		{
			const char* name = server->Attribute("name");
			if (name != NULL)
				m_name = name;

			tinyxml2::XMLElement* settingNode = server->FirstChildElement("setting");
			while (settingNode != NULL)
			{
				std::string settingName = settingNode->Attribute("name");
				if (settingName == "min-players")
					settingNode->QueryFloatText(&m_minPlayers);
				else if (settingName == "max-players")
					settingNode->QueryFloatText(&m_maxPlayers);
				else if (settingName == "round-duration")
					settingNode->QueryFloatText(&m_roundDuration);
				else if (settingName == "round-intermission")
					settingNode->QueryFloatText(&m_roundIntermission);
				else if (settingName == "inactive-timeout")
					settingNode->QueryFloatText(&m_inactiveTimeout);
				else if (settingName == "language")
					m_language = settingNode->GetText();

				settingNode = settingNode->NextSiblingElement("setting");
			}
		} else {
			cpp3ds::err() << "No \"server\" element defined in configuration file. Using default settings.";
		}
	} else {
		cpp3ds::err() << "Failed to load configuration file. Using default settings.";
	}
}

const std::string &ServerConfig::getServerName() const
{
	return m_name;
}

const std::string &ServerConfig::getLanguage() const
{
	return m_language;
}

const float ServerConfig::getMinPlayers() const
{
	return m_minPlayers;
}

const float ServerConfig::getMaxPlayers() const
{
	return m_maxPlayers;
}

const float ServerConfig::getRoundDuration() const
{
	return m_roundDuration;
}

const float ServerConfig::getRoundIntermission() const
{
	return m_roundIntermission;
}

const float ServerConfig::getInactiveTimeout() const
{
	return m_inactiveTimeout;
}

} // namespace DrawAttack
