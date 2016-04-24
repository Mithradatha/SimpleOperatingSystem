# include "OperatingSystem.h"

OperatingSystem::OperatingSystem() {

	OperatingSystem("my_os_config_" + string(to_string(time(0))) + ".txt");
}

OperatingSystem::OperatingSystem(string path) {

	this->addFileType(FileHandler::FileType::DOCUMENT);
	this->addFileType(FileHandler::FileType::MUSIC);
	this->addFileType(FileHandler::FileType::PICTURE);
	this->setPath(path);
}

bool OperatingSystem::isValidExtension(string str, string extension) {

	if (str.length() < 4) return false;
	return (str.substr(str.length() - 4, 4) == extension);
}

bool OperatingSystem::isInstalled(string deviceName) {

	return (this->devices.find(deviceName) != this->devices.end());
}

bool OperatingSystem::isOperable(const type_info& left, const type_info& right) {

	return left.hash_code() == right.hash_code();
}

bool OperatingSystem::installDeviceDriver(string deviceName, int type, string path) {

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

//bool OperatingSystem::uninstallDeviceDriver(string deviceName, int type, string path);

void OperatingSystem::initialize() {

	fstream mFile;
	mFile.open(this->getPath(), ios::in);

	if (mFile.is_open()) {
		string line;
		while (getline(mFile, line)) {
			
			if (line.find("/*") != string::npos) {

				while (line.find("*/") == string::npos) {
					getline(mFile, line);
					if (line == "*/") break;
					size_t begin = line.find_first_not_of('{');
					string deviceName = line.substr(begin, line.find_first_of('}') - 1);
					begin = line.find_first_of('(');
					if (begin == string::npos) {
						this->installDeviceDriver(deviceName, 2, "");
					}
					else {
						string path = line.substr(begin + 1, line.find_first_of(')') - (begin + 1));
						this->installDeviceDriver(deviceName, 1, path);
					}
				}
			}
		}

		mFile.close();
	}

	this->update(FileHandler::pleb_key(*this));
}

void OperatingSystem::uninitialize() {

	fstream mFile(this->getPath(), ios::in);
	size_t begin;

	if (mFile.is_open()) {

		string line;
		while (getline(mFile, line)) {
			begin = line.find("/*");
		}

		mFile.close();

		ofstream strm;
		if (begin == string::npos) {
			strm.open(this->getPath(), ios::app);
		}
		else {
			strm.open(this->getPath());
			strm.seekp(begin);
		}
		
		strm << "/*" << endl;

		for (unordered_map<string, shared_ptr<Device>>::iterator itr = this->devices.begin(); itr != this->devices.end(); ++itr) {

			shared_ptr<Operations::Syncable> phone = dynamic_pointer_cast<Operations::Syncable>(itr->second);
			if (phone) {
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

	if (isInstalled(deviceName)) {

		if (this->devices[deviceName]->isConnected()) {
			cout << "Device Already Connected" << endl;
		}
		else {
			this->devices[deviceName]->setConnection(true);
			cout << "Device Connected" << endl;
		}
	}
	else {
		cout << "Device Not Recognized" << endl
			<< "Would You Like To Install The Device Driver? (Y/n): ";
		if (char input = tolower(cin.get()) == 'y') {
			cin.ignore(256, '\n');
			cout << "1. PHONE" << endl
				<< "2. PRINTER" << endl
				<< "Enter Device Type: ";
			int num;
			cin >> num;
			switch (num) {
			case 1:
				cin.ignore(256, '\n');
				cout << "Do You Already Have A Configuration File? (Y/n): ";
				if (char input = tolower(cin.get()) == 'y') {
					cin.ignore(256, '\n');
					cout << "Enter the Phone's configuration file name: ";
					string path;
					getline(cin, path);
					if (isValidExtension(path, ".txt")) {
						
						if (installDeviceDriver(deviceName, 1, path)) {
							this->devices[deviceName]->setConnection(true);
							cout << "Device Connected" << endl;
						}
					}
					else {
						cout << "Invalid Extension: Must Be \'.txt\' Extension" << endl;
					}
				}
				else {
					cin.ignore(256, '\n');
					if (installDeviceDriver(deviceName, 1, "")) {
						this->devices[deviceName]->setConnection(true);
						cout << "Device Connected" << endl;
					}
				}
				break;
			case 2:
				cin.ignore(256, '\n');
				if (installDeviceDriver(deviceName, 2, "")) {
					this->devices[deviceName]->setConnection(true);
					cout << "Device Connected" << endl;
				}
				break;
			default:
				break;
			}
		}
		else {
			cin.ignore(256, '\n');
		}
	}
}

void OperatingSystem::disconnectDevice(string deviceName) {

	if (isInstalled(deviceName)) {

		if (this->devices[deviceName]->isConnected()) {
			this->devices[deviceName]->setConnection(false);
			cout << "Device Disconnected" << endl;
		}
		else {
			cout << "Device Already Disconnected" << endl;
		}
	}
	else {
		cout << "Device Not Recognized" << endl;
	}
}

void OperatingSystem::displayInstalledDevices() {

	for (unordered_map<string, shared_ptr<Device>>::iterator itr = this->devices.begin(); itr != this->devices.end(); ++itr) {
		cout << itr->first << " is a ";
		itr->second->display();
	}
}

void OperatingSystem::displayConnectedDevices() {

	for (unordered_map<string, shared_ptr<Device>>::iterator itr = this->devices.begin(); itr != this->devices.end(); ++itr) {
		
		if (itr->second->isConnected()) {
			cout << itr->first << " is a ";
			itr->second->display();
		}
	}
}

void OperatingSystem::onSync(string deviceName, FileHandler::FileType type) {

	if (isInstalled(deviceName)) {

		if (this->devices[deviceName]->isConnected()) {

			shared_ptr<Operations::Syncable> ptr = dynamic_pointer_cast<Operations::Syncable>(this->devices[deviceName]);
			if (ptr) {

				unordered_map<FileHandler::FileType, forward_list<string>> os = FileHandler::pleb_key(*this);
				unordered_map<FileHandler::FileType, forward_list<string>> device = FileHandler::pleb_key(*ptr);
				ptr->sync(ptr->update(FileHandler::diff_key(FileHandler::master_key(os, device), device, type)));
				ptr->sync(this->update(FileHandler::diff_key(FileHandler::master_key(os, device), os, type)));
				cout << "Finished Syncing" << endl;
			}
		}
		else {
			cout << "Device Not Connected" << endl;
		}
	}
	else {
		cout << "Device Not Recognized" << endl;
	}
}

void OperatingSystem::onScan(string deviceName, string fileName) {
	
	
	if (isInstalled(deviceName)) {

		if (this->devices[deviceName]->isConnected()) {

			if (isValidExtension(fileName, ".doc") || isValidExtension(fileName, ".jpg")) {

				if (!this->contains(fileName)) {

					shared_ptr<Operations::Scannable> ptr = dynamic_pointer_cast<Operations::Scannable>(this->devices[deviceName]);
					if (ptr) {

						FileHandler::FileType type = FileHandler::toFileType(fileName.substr(fileName.length() - 4, 4));
						unordered_map<FileHandler::FileType, forward_list<string>> os = FileHandler::pleb_key(*this);
						os[type].push_front(fileName);
						this->update(os);
						ptr->scan(fileName, type);
					}
				}
				else {
					cout << fileName << " Already Exists" << endl;
				}
			}
			else {
				cout << "Invalid Extension: " << endl;
			}
		}
		else {
			cout << "Device Not Connected" << endl;
		}
	}
	else {
		cout << "Device Not Recognized" << endl;
	}
}

void OperatingSystem::onPrint(string deviceName, string fileName) {

	if (isInstalled(deviceName)) {

		if (this->devices[deviceName]->isConnected()) {

			if (isValidExtension(fileName, ".doc") || isValidExtension(fileName, ".jpg")) {

				if (this->contains(fileName)) {

					shared_ptr<Operations::Printable> ptr = dynamic_pointer_cast<Operations::Printable>(this->devices[deviceName]);
					if (ptr) {

						ptr->print(fileName, FileHandler::toFileType(fileName.substr(fileName.length() - 4, 4)));
					}
				}
				else {
					cout << "File Not Found" << endl;
				}
			}
			else {
				cout << "Invalid Extension: " << endl;
			}
		}
		else {
			cout << "Device Not Connected" << endl;
		}
	}
	else {
		cout << "Device Not Recognized" << endl;
	}
}