#include "Phone.h"

int Phone::count = 0;


Phone::Phone() {

	Phone("my_phone_config_" + string(to_string(count)) + ".txt");
}

Phone::Phone(string path) { 
	
	this->addFileType(FileHandler::FileType::MUSIC);
	this->addFileType(FileHandler::FileType::PICTURE);
	this->setConnection(false);
	++count; 
	this->setPath(path);
}

void Phone::display() {

	cout << "PHONE" << endl;
}

void Phone::sync(unordered_map<FileHandler::FileType, int> updates) {

	for (unordered_map<FileHandler::FileType, int>::iterator itr = updates.begin(); itr != updates.end(); ++itr) {

		if (itr->second > 0) RNGesus::initialize(itr->second, itr->first, "Syncing... ");
	}
}