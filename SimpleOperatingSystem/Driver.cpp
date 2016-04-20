#include "OperatingSystem.h"
#include <iostream>

int main() {

	OperatingSystem os("SystemConfigFile.txt");
	os.initialize();

	os.displayInstalledDevices();
	
	os.uninitialize();

	return 0;
}