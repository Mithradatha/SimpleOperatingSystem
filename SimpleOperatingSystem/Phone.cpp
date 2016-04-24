#include "Phone.h"

Phone::Phone(string path) { 
	
	this->addFileType(FileHandler::FileType::MUSIC);
	this->addFileType(FileHandler::FileType::PICTURE);
	this->setConnection(false);

	// only set the path after all the allowed file types have been added
	this->setPath(path);
}

void Phone::display() {

	cout << "PHONE" << endl;
}

void Phone::sync(unordered_map<FileHandler::FileType, int> updates) {

	// for each updated FileType in the list, initiate the random synchronization process
	for (unordered_map<FileHandler::FileType, int>::iterator itr = updates.begin(); itr != updates.end(); ++itr) {

		if (itr->second > 0) RNGesus::initialize(itr->second, itr->first, "Syncing... ");
	}
}