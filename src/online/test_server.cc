
#include "Configuration.h"
#include "WordQueryServer.h"

#include <iostream>

using namespace std;
using namespace mysp;


int main()
{

	Configuration::getInstance("../../conf/server.conf");
	WordQueryServer server;
	server.start();

	return 0;
}
