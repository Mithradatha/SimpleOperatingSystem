# include "OperatingSystem.h"

OperatingSystem::OperatingSystem(string path) {

	this->addFileType(FileHandler::FileType::DOCUMENT);
	this->addFileType(FileHandler::FileType::MUSIC);
	this->addFileType(FileHandler::FileType::PICTURE);

	// only set the path after all the allowed file types have been added
	this->setPath(path);
}

bool OperatingSystem::isValidExtension(string str, string extension) {

	if (str.length() < 4) return false;
	return (str.substr(str.length() - 4, 4) == extension);
}

bool OperatingSystem::isInstalled(string deviceName) {

	return (this->devices.find(deviceName) != this->devices.end());
}

//bool OperatingSystem::isOperable(const type_info& left, const type_info& right) {
//
//	return left.hash_code() == right.hash_code();
//}

bool OperatingSystem::installDeviceDriver(string deviceName, int type, string path) {

	/*
	* type '1' = Phone
	* type '2' = Printer
	*/

	// TODO: Replace 'new $Device' with 'make_unique<$Device>()'
	// TODO: Come up with a better way to simulate device driver installation than using RNGesus::initialize(FileType::PICTURE)

	switch (type) {
	case 1:
		if (path.length() > 1) { this->devices.insert(make_pair(deviceName, shared_ptr<Device>(new Phone(path)))); }
		else { this->devices.insert(make_pair(deviceName, shared_ptr<Device>(new Phone("my_phone_config_" + string(to_string(time(0))) + ".txt")))); }
		RNGesus::initialize(1, FileHandler::FileType::PICTURE, "Installing Phone Device Driver... ");
		cout << "Installed " << deviceName << endl;
		return true;
	case 2:
		this->devices.insert(make_pair(deviceName, shared_ptr<Device>(new Printer)));
		RNGesus::initialize(1, FileHandler::FileType::PICTURE, "Installing Printer Device Driver... ");
		cout << "Installed " << deviceName << endl;
		return true;
	default:
		cout << "Invalid Device Type" << endl;
		return false;
	}
}

// TODO: Implement this :)
//bool OperatingSystem::uninstallDeviceDriver(string deviceName, int type, string path);


// installs all previously installed device drivers
void OperatingSystem::initialize() {

	fstream mFile;
	mFile.open(this->getPath(), ios::in);	// open the configuration file for reading

	if (mFile.is_open()) {
		string line;
		while (getline(mFile, line)) {
			
			if (line.find("/*") != string::npos) {

				while (line.find("*/") == string::npos) {
					getline(mFile, line);
					if (line == "*/") break;
					size_t begin = line.find_first_not_of('{');
					string deviceName = line.substr(begin, line.find_first_of('}') - 1);	// get the name of the device
					begin = line.find_first_of('(');	// if the device has parenthesis, then it has a configuration file
					if (begin == string::npos) {
						this->installDeviceDriver(deviceName, 2, "");	// if it doesn't have a path, it's a Printer
					}
					else {
						string path = line.substr(begin + 1, line.find_first_of(')') - (begin + 1));	// get the path
						this->installDeviceDriver(deviceName, 1, path);	// if it does have a path, its a Phone
					}
				}
			}
		}

		mFile.close();
	}

	// TODO: Figure out how to stop the configuration file from appending "/**\" over and over again,
	//		 so that this (below) can be removed...	Possible Solution: combine ios::trunc with seekp(pos)
	this->update(FileHandler::pleb_key(*this));
}

// adds the initialization list into the operating system's configuration file based on all currently installed device drivers
void OperatingSystem::uninitialize() {

	fstream mFile(this->getPath(), ios::in);	// open the configuration file for reading
	size_t begin;

	if (mFile.is_open()) {

		string line;
		while (getline(mFile, line)) {
			begin = line.find("/*");
		}

		mFile.close();

		ofstream strm;
		if (begin == string::npos) {	// if there is no instance of an initialization list already in the file
			strm.open(this->getPath(), ios::app);	// open the configuration file for writing, by appending
		}
		else {	// if there is an instance of an intialization list already in the file
			strm.open(this->getPath());	// open the configuration file for writing
			strm.seekp(begin);	// set the output stream position to the position of the "/*" in the intitialization list
		}
		
		strm << "/*" << endl;

		// for each currently installed device, add it's name and possible configuration file path to the intialization list
		for (unordered_map<string, shared_ptr<Device>>::iterator itr = this->devices.begin(); itr != this->devices.end(); ++itr) {

			shared_ptr<Operations::Syncable> phone = dynamic_pointer_cast<Operations::Syncable>(itr->second);
			if (phone) {
				// if the device is 'Syncable', then it is also 'Configurable', so it has a file path
				strm << "{" << itr->first << "}" << "(" << phone->getPath() << ")" << endl;
			}
			else {
				shared_ptr<Operations::Printable> printer = dynamic_pointer_cast<Operations::Printable>(itr->second);
				if (printer) {
					strm << "{" << itr->first << "}" << endl;
				}
			}
		}

		strm << "*/" << endl;

		strm.close();
	}
}


void OperatingSystem::connectDevice(string deviceName) {

	if (isInstalled(deviceName)) {	// if the device is installed

		if (this->devices[deviceName]->isConnected()) {	// if the device is already connected
			cout << "Device Already Connected" << endl;
		}
		else {	// if the device is not already connected
			this->devices[deviceName]->setConnection(true);
			cout << "Device Connected" << endl;
		}
	}
	else {	// if the device is not installed
		cout << "Device Not Recognized" << endl
			<< "Would You Like To Install The Device Driver? (Y/n): ";
		// if the user would like to install the device driver
		if (char input = tolower(cin.get()) == 'y') {
			cin.ignore(256, '\n');
			cout << "1. PHONE" << endl
				<< "2. PRINTER" << endl
				<< "Enter Device Type: ";
			int num;
			cin >> num;
			switch (num) {	// num '1' = Phone; num '2' = Printer

			// Phone
			case 1:
				cin.ignore(256, '\n');
				cout << "Do You Already Have A Configuration File? (Y/n): ";
				// if the user has already made a configuration file
				if (char input = tolower(cin.get()) == 'y') {
					cin.ignore(256, '\n');
					cout << "Enter the Phone's configuration file name: ";
					string path;
					getline(cin, path);
					if (isValidExtension(path, ".txt")) {	// verify that the path refers to a text file
						
						if (installDeviceDriver(deviceName, 1, path)) {	// install a Phone device driver
							this->devices[deviceName]->setConnection(true);	// connect the device
							cout << "Device Connected" << endl;
						}
					}
					else {	// if the path does not refer to a text file
						cout << "Invalid Extension: Must Be \'.txt\' Extension" << endl;
					}
				}
				else {	// if the user has not yet made a configuration file
					cin.ignore(256, '\n');
					if (installDeviceDriver(deviceName, 1, "")) {	// install a Phone device driver
						this->devices[deviceName]->setConnection(true);	// connect the device
						cout << "Device Connected" << endl;
					}
				}
				break;

			// Printer
			case 2:
				cin.ignore(256, '\n');
				if (installDeviceDriver(deviceName, 2, "")) {	// install a Printer device driver
					this->devices[deviceName]->setConnection(true);	// connect the device
					cout << "Device Connected" << endl;
				}
				break;
			
			// if user enters NOT '1' or '2', do nothing
			default:
				// TODO: Throw something here
				break;
			}
		}
		// if the user would not like to install the device driver
		else {
			cin.ignore(256, '\n');
		}
	}
}

void OperatingSystem::disconnectDevice(string deviceName) {

	if (isInstalled(deviceName)) {	// if the device is currently installed

		if (this->devices[deviceName]->isConnected()) {	// if the device is connected
			this->devices[deviceName]->setConnection(false);	// disconnect the device
			cout << "Device Disconnected" << endl;
		}
		else {	// if the device is already disconnected
			cout << "Device Already Disconnected" << endl;
		}
	}
	else {	// if the device is not installed
		cout << "Device Not Recognized" << endl;
	}
}

// displays all the curretly installed devices
void OperatingSystem::displayInstalledDevices() {

	// for each device in the list of devices: call the pure virtual display function on each Device
	for (unordered_map<string, shared_ptr<Device>>::iterator itr = this->devices.begin(); itr != this->devices.end(); ++itr) {
		cout << itr->first << " is a ";
		itr->second->display();
	}
}

// displays only those currently installed devices which are also connected
void OperatingSystem::displayConnectedDevices() {

	// for each device in the list of devices: call the pure virtual display function on each Device, if it is also connected
	for (unordered_map<string, shared_ptr<Device>>::iterator itr = this->devices.begin(); itr != this->devices.end(); ++itr) {
		
		if (itr->second->isConnected()) {
			cout << itr->first << " is a ";
			itr->second->display();
		}
	}
}

void OperatingSystem::onSync(string deviceName, FileHandler::FileType type) {

	if (isInstalled(deviceName)) {	// if the device is currently installed

		if (this->devices[deviceName]->isConnected()) {	// if the device is connected

			// downcast the device to the interface that it has presumably implented
			shared_ptr<Operations::Syncable> ptr = dynamic_pointer_cast<Operations::Syncable>(this->devices[deviceName]);
			if (ptr) {	// if the cast succeeds

				// TODO: Make this (below) less messy, more readable

				unordered_map<FileHandler::FileType, forward_list<string>> os = FileHandler::pleb_key(*this);	// os pleb_key
				unordered_map<FileHandler::FileType, forward_list<string>> device = FileHandler::pleb_key(*ptr);	// device pleb_key

				// sync(unordered_map<FileType, int> updates)
				// [from]: update(unordered_map<FileType, forward_list<string>> master)
				// [where]: master [from]: diff_key(unordered_map<FileType, forward_list<string>> master,
				// unordered_map<FileType, forward_list<string>> pleb_key, FileType type)
				// [where]: master
				// [from]: master_key(unordered_map<FileType, forward_list<string>> pleb_key1, unordered_map<FileType, 
				// forward_list<string>> pleb_key2)
				// [where]: pleb_key
				// [from]: os/device
				// [and]: pleb_key1
				// [from]: os
				// [and]: pleb_key2 
				// [from]: device

				// sync and update device
				ptr->sync(ptr->update(FileHandler::diff_key(FileHandler::master_key(os, device), device, type)));
				
				cout << "Syncing " << deviceName << " Completed..." << endl;

				// sync and update operating system
				ptr->sync(this->update(FileHandler::diff_key(FileHandler::master_key(os, device), os, type)));

				cout << "Syncing Operating System Completed..." << endl;
				
				cout << "Finished Syncing" << endl;
			}
		}
		else {	// if the device is not yet connected
			cout << "Device Not Connected" << endl;
		}
	}
	else {	// if the device driver is not installed
		cout << "Device Not Recognized" << endl;
	}
}

void OperatingSystem::onScan(string deviceName, string fileName) {
	
	
	if (isInstalled(deviceName)) {	// if the device driver is installed

		if (this->devices[deviceName]->isConnected()) {	// if the device is connected

			// Only DOCUMENT and PICTURE file types can be scanned
			if (isValidExtension(fileName, ".doc") || isValidExtension(fileName, ".jpg")) {

				if (!this->contains(fileName)) {	// if the configuration file doesn't already contain the file name

					// downcast the device to the interface that it has presumably implented
					shared_ptr<Operations::Scannable> ptr = dynamic_pointer_cast<Operations::Scannable>(this->devices[deviceName]);
					if (ptr) {	// if the cast succeeds

						// get the file type
						FileHandler::FileType type = FileHandler::toFileType(fileName.substr(fileName.length() - 4, 4));
						unordered_map<FileHandler::FileType, forward_list<string>> os = FileHandler::pleb_key(*this);	// os pleb_key
						
						// No need to worry about using FileHandler::diff_key, because only 1 file will ever be updated
						os[type].push_front(fileName);
						this->update(os);	// update the operating system's configuration file
						ptr->scan(fileName, type);
					}
					else {	// if the cast fails
						cout << deviceName << " Not Scannable" << endl;
					}
				}
				else {	// if the configuration file already contains the file name
					cout << fileName << " Already Exists" << endl;
				}
			}
			else {	// if the extension is not ".doc" or ".jpg"
				cout << "Invalid Extension: " << endl;
			}
		}
		else {	// if the device is not yet connected
			cout << "Device Not Connected" << endl;
		}
	}
	else {	// if the device driver is not installed
		cout << "Device Not Recognized" << endl;
	}
}

void OperatingSystem::onPrint(string deviceName, string fileName) {

	if (isInstalled(deviceName)) {	// if the device is installed

		if (this->devices[deviceName]->isConnected()) {	// if the device is currently connected

			// Only DOCUMENT and PICTURE file types can be printed
			if (isValidExtension(fileName, ".doc") || isValidExtension(fileName, ".jpg")) {

				if (this->contains(fileName)) {	// if the configuration file already contains the file name

					// downcast the device to the interface that it has presumably implented
					shared_ptr<Operations::Printable> ptr = dynamic_pointer_cast<Operations::Printable>(this->devices[deviceName]);
					if (ptr) {	// if the cast succeeds

						ptr->print(fileName, FileHandler::toFileType(fileName.substr(fileName.length() - 4, 4)));
					}
					else {	// if the cast fails
						cout << deviceName << " Not Printable" << endl;
					}
				}
				else {	// if the configuration file doesn't already contain the file name
					cout << "File Not Found" << endl;
				}
			}
			else {	// if the extension is not ".doc" or ".jpg"
				cout << "Invalid Extension: " << endl;
			}
		}
		else {	// if the device is not yet connected
			cout << "Device Not Connected" << endl;
		}
	}
	else {	// if the device driver is not installed
		cout << "Device Not Recognized" << endl;
	}
}
