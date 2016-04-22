#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H

#include <memory>
#include <ctime>
#include "Device.h"
#include "Phone.h"
#include "Printer.h"
#include "FileHandler.h"

using namespace std;

class OperatingSystem : public FileHandler::Configurable {

private:

	unordered_map<string, shared_ptr<Device>> devices;

	bool isValidExtension(string str, string extension);
	bool isInstalled(string deviceName);

public:

	OperatingSystem();
	OperatingSystem(string path);

	void initialize();
	void uninitialize();

	bool installDeviceDriver(string deviceName, int type, string path);
	bool uninstallDeviceDriver(string deviceName, int type, string path);

	void connectDevice(string deviceName);
	void disconnectDevice(string deviceName);

	void displayInstalledDevices();
	void displayConnectedDevices();

	void onSync(string deviceName, FileHandler::FileType type);
	void onScan(string deviceName, string fileName);
	void onPrint(string deviceName, string fileName);
};
#endif
