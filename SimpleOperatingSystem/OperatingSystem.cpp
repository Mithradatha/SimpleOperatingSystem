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

bool OperatingSystem::installDeviceDriver(string deviceName, int type, string path) {

	switch (type) {
	case 1:
		if (path.length() > 1) { this->devices.insert(make_pair(deviceName, shared_ptr<Device>(new Phone(path)))); }
		else { this->devices.insert(make_pair(deviceName, shared_ptr<Device>(new Phone))); }
		cout << "Installing Phone Device Driver... " << deviceName << endl;
		return true;
	case 2:
		this->devices.insert(make_pair(deviceName, shared_ptr<Device>(new Printer)));
		cout << "Installing Printer Device Driver... " << deviceName << endl;
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
					if (!isValidExtension(path, ".txt")) {
						cout << "Invalid Extension!" << endl;
					}
					if (installDeviceDriver(deviceName, 1, path)) {
						this->devices[deviceName]->setConnection(true);
						cout << "Device Connected" << endl;
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
//void OperatingSystem::disconnectDevice(string deviceName);
//
void OperatingSystem::displayInstalledDevices() {

	for (unordered_map<string, shared_ptr<Device>>::iterator itr = this->devices.begin(); itr != this->devices.end(); ++itr) {
		cout << itr->first << " is a ";
		itr->second->display();

		shared_ptr<Operations::Syncable> ptr = dynamic_pointer_cast<Operations::Syncable>(itr->second);
		if (ptr) {

			unordered_map<FileHandler::FileType, forward_list<string>> master = FileHandler::master_key(FileHandler::pleb_key(*ptr), FileHandler::pleb_key(*this));

			for (unordered_map<FileHandler::FileType, forward_list<string>>::iterator masterITR = master.begin(); masterITR != master.end(); ++masterITR) {

				forward_list<string> changed = masterITR->second;
				cout << FileHandler::toString(masterITR->first) << ": [";
				bool first = true;

				for (forward_list<string>::iterator innerITR = changed.begin(); innerITR != changed.end(); ++innerITR) {
					if (first) { cout << *innerITR; first = false; }
					else cout << ", " << *innerITR;
				}
				cout << "]" << endl;
			}

			ptr->sync(ptr->update(master));
			this->update(master);
		}

		/*shared_ptr<Operations::Syncable> ptr = dynamic_pointer_cast<Operations::Syncable>(itr->second);
		if (ptr) {

		unordered_set<FileHandler::FileType> set = ptr->getFileTypes();
		for (unordered_set<FileHandler::FileType>::iterator pItr = set.begin(); pItr != set.end(); ++pItr) {
		cout << FileHandler::toString(*pItr) << endl;
		}

		unordered_map<string, forward_list<string>> files = FileHandler::pleb_key(*ptr);
		for (unordered_map<string, forward_list<string>>::iterator mItr = files.begin(); mItr != files.end(); ++mItr) {

		forward_list<string> changed = mItr->second;
		cout << mItr->first << ": [";
		bool first = true;

		for (forward_list<string>::iterator fItr = changed.begin(); fItr != changed.end(); ++fItr) {
		if (first) { cout << *fItr; first = false; }
		else cout << ", " << *fItr;
		}
		cout << "]" << endl;
		}*/
	}
}
//void OperatingSystem::displayConnectedDevices();

void OperatingSystem::onSync(string deviceName) {

}

void OperatingSystem::onScan(string deviceName, string fileName) {

}

void OperatingSystem::onPrint(string deviceName, string fileName) {


}