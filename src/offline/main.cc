
#include "Configuration.h"
#include "PageLib.h"
#include "PageLibPreProcessor.h"

using namespace mysp;

int main(){
	mysp::Configuration::getInstance("../../conf/offline.conf");

	PageLib pagelib;
	pagelib.create();
	pagelib.store();

	PageLibPreProcessor processor;
	processor.doProcess();
	processor.storeOnDisk();

	return 0;
}
