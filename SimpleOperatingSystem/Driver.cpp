/*
* Name: Samuel Kellar
* Email: skellar2014@my.fit.edu
* Purpose:
*	Manage peripheral devices using a simple simulation of an operating system
*	whose responsibilities include installing/uninstalling device drivers,
*	displaying connected/installed devices, and communicating with those devices.
* Algorithm:
*	A) Initialize an operating system based upon it's configuration file
*	B) Install all device drivers that are necessary for device communication
*	C) Connect Devices that are already installed
*	D) Communicate with connected devices in 3 ways:
*		1. Synchronize files between a Phone and the Operating System
*		2. Scan a file from a Printer into the Operating System's configuratin file
*		3. Print a file from the Operating System's configuration file to the Printer
*	E) Unintialize an operating system based upon currenly installed device drivers
*/

// TODO: Refractor most of the iterator 'for' loops to 'for each' loops
// TODO: Go through all the functions and pass 'const' where applicable, and references as well
// TODO: Fix Formatting (Add more white space; Use better variable names; Rename functions)

#include "OperatingSystem.h"
#include <iostream>

void main_menu(const string& sysConfig);

int main() {

	string sysConfig;
	bool isValid = false;
	do {
		cout << "Enter System Configuration File Name: ";
		getline(cin, sysConfig);

		if (sysConfig.substr(sysConfig.length() - 4, 4) != ".txt") {
			cout << "Invalid File Name: File Must Have \'.txt\' Extension" << endl;
		}
		else isValid = true;

	} while (!isValid);

	main_menu(sysConfig);
	
	return 0;
}

void main_menu(const string& sysConfig) {

	OperatingSystem os(sysConfig);
	os.initialize();	// installs all previously installed device drivers
	
	bool outerRunning = true;

	while (outerRunning) {

		cout <<
			"1. Connect Device" << endl <<
			"2. Disconnect Device" << endl <<
			"3. Display Installed Devices" << endl <<
			"4. Display Connected Devices" << endl <<
			"5. Communicate With Device" << endl <<
			"6. Exit" << endl <<

			"> ";

		int input;
		cin >> input;
		cin.ignore(256, '\n');

		switch (input) {

		case 1:
		{
				  cout << "Device Name: ";
				  string deviceName;
				  getline(cin, deviceName);

				  // connects an already installed device to the operating system, 
				  // or asks the user if they would like to install the device driver
				  os.connectDevice(deviceName);
		};
			break;
		case 2:
		{
				  cout << "Device Name: ";
				  string deviceName;
				  getline(cin, deviceName);

				  os.disconnectDevice(deviceName);	// disconnects the device from the operating system
		};
			break;
		case 3:
			os.displayInstalledDevices();	// displays all the currently installed devices
			break;
		case 4:
			os.displayConnectedDevices();	// displays all the currently connected devices
			break;
		case 5:
		{
				  bool innerRunning = true;

				  while (innerRunning) {

					  cout <<
						  "1. Sync" << endl <<
						  "2. Print" << endl <<
						  "3. Scan" << endl <<
						  "4. Back" << endl <<

						  "> ";

					  int _input;
					  cin >> _input;
					  cin.ignore(256, '\n');

					  switch (_input) {

					  case 1:
					  {
								cout << "Device Name: ";
								string deviceName;
								getline(cin, deviceName);

								// TODO: Validate Input; convert lowercase/uppercase
								cout << "File Type (MUSIC, PHOTOS): ";
								string fileType;
								getline(cin, fileType);

								FileHandler::FileType type = FileHandler::toFileType(fileType);

								// sends the sync event to the operating system,
								// and syncs between a device and the operating system
								os.onSync(deviceName, type);
					  };
						  break;
					  case 2:
					  {
								cout << "Device Name: ";
								string deviceName;
								getline(cin, deviceName);

								cout << "File Name: ";
								string fileName;
								getline(cin, fileName);

								// sends the print event to the operating system,
								// and prints a file from the operating system to the device
								os.onPrint(deviceName, fileName);
					  };
						  break;
					  case 3:
					  {
								cout << "Device Name: ";
								string deviceName;
								getline(cin, deviceName);

								cout << "File Name: ";
								string fileName;
								getline(cin, fileName);

								// sends a scan even to the operating system,
								// and scans a file from a device to the operating system
								os.onScan(deviceName, fileName);
					  };
						  break;
					  case 4:
						  innerRunning = false;
						  break;
					  }
				  };
				  break;
		};
		case 6:
			// updates the operating system's configuration file to include
			// all currently installed device drivers
			os.uninitialize();
			outerRunning = false;
			break;
		default:
			break;
		}
	}
}
