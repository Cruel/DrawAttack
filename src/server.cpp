#include <signal.h>
#include "Network/Server.hpp"

DrawAttack::Server* server;

static void sighandler(int sig)
{
	std::string reason;
	std::cout << std::endl << "Reason for shutdown: ";
	std::getline(std::cin, reason);
	std::cout << "Shutting down server..." << std::endl;
	server->exit(reason);
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		std::cout << "Usage: " << argv[0] << " {port} {word file}" << std::endl;
		return 0;
	}

	std::srand(std::time(nullptr));
	signal(SIGTERM, &sighandler);
	signal(SIGINT,  &sighandler);

	server = new DrawAttack::Server(atoi(argv[1]), argv[2]);
	server->run();
	delete server;

	return 0;
}
