#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H

#include <typeinfo>
#include <memory>
#include <ctime>
#include "Device.h"
#include "Phone.h"
#include "Printer.h"
#include "FileHandler.h"

/*
* Operating System derives from Configurable, so it has a configuration file and a set of allowed file types
*
* It's responsible for handling user events including:
*
*	1) Installing/Uninstalling device drivers
*	2) Connecting/Disconnecting from installed devices
*	3) Displaying installed/connected devices
*	4) Communicating with devices:
*		1. Syncing
*		2. Scanning
*		3. Printing
*/

// TODO: Replace most of the if/else blocks with try/catch, and throw specific errors

using namespace std;

class OperatingSystem : public FileHandler::Configurable {

private:

	// TODO: devices should use unique_ptr, because each device is only refrenced once and the shared_ptr counter creates overhead
	unordered_map<string, shared_ptr<Device>> devices;

	bool isValidExtension(string str, string extension);	// check to see if the file name has a proper extension
	bool isInstalled(string deviceName);	// check to see if the passed device is currently installed
	
	// TODO: implement the function below, such that it can verify that a device implements a given 'Operatons' interface
	//bool isOperable(const type_info& left, const type_info& right);

public:

	OperatingSystem(string path);

	void initialize();	// initialize the operating system's previously installed devices from the configuration file
	void uninitialize();	// add the currently installed device drivers to the operating system's configuration file

	bool installDeviceDriver(string deviceName, int type, string path);	// install a device driver
	//bool uninstallDeviceDriver(string deviceName, int type, string path);	// uninstall a currently installed device driver

	void connectDevice(string deviceName);	// connect a currently installed device
	void disconnectDevice(string deviceName);	// disconnect a currently connected device

	void displayInstalledDevices();	// display all the installed devices
	void displayConnectedDevices();	// display all the connected and installed devices

	// synchronize the operating system's configuration file with the device's configuration file
	void onSync(string deviceName, FileHandler::FileType type);

	// scan a file from a device into the operating system's configuration file
	void onScan(string deviceName, string fileName);

	// print a file from the operating system's configuration file to the device
	void onPrint(string deviceName, string fileName);
};
#endif
