/*
* Name: Samuel Kellar
* Email: skellar2014@my.fit.edu
*
*/



#include "OperatingSystem.h"
#include <iostream>

void main_menu();

int main() {

	main_menu();
	
	return 0;
}

void main_menu() {

	OperatingSystem os("SystemConfigFile.txt");
	os.initialize();
	
	bool outerRunning = true;

	while (outerRunning) {

		cout <<
			"1. Connect Device" << endl <<
			"2. Disconnect Device" << endl <<
			"3. Display Installed Devices" << endl <<
			"4. Display Connected Devices" << endl <<
			"5. Communicate with Device" << endl <<
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

				  os.connectDevice(deviceName);
		};
			break;
		case 2:
		{
				  cout << "Device Name: ";
				  string deviceName;
				  getline(cin, deviceName);

				  os.disconnectDevice(deviceName);
		};
			break;
		case 3:
			os.displayInstalledDevices();
			break;
		case 4:
			os.displayConnectedDevices();
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

								cout << "File Type (MUSIC, PICTURE): ";
								string fileType;
								getline(cin, fileType);

								FileHandler::FileType type = FileHandler::toFileType(fileType);

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
			os.uninitialize();
			outerRunning = false;
			break;
		default:
			cout << "Daferq" << endl;
			break;
		}
	}
}
