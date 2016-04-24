#include "FileHandler.h"

using namespace std;

namespace FileHandler {
	string toString(FileType fileType) {

		if (fileType == FileType::DOCUMENT) {
			return "DOCUMENT";
		}
		else if (fileType == FileType::MUSIC) {
			return "MUSIC";
		}
		else if (fileType == FileType::PICTURE) {
			return "PICTURE";
		}
		else {
			return "";	// TODO: Should throw an exception
		}
	}

	string toExtension(FileType fileType) {

		if (fileType == FileType::DOCUMENT) {
			return ".doc";
		}
		else if (fileType == FileType::MUSIC) {
			return ".mp3";
		}
		else if (fileType == FileType::PICTURE) {
			return ".jpg";
		}
		else {
			return "";	// TODO: Should throw an exception
		}
	}

	FileType toFileType(string str) {

		if (str == "DOCUMENT" || str == ".doc") {
			return FileType::DOCUMENT;
		}
		else if (str == "MUSIC" || str == ".mp3") {
			return FileType::MUSIC;
		}
		else if (str == "PICTURE" ||  str == "PHOTOS" || str == ".jpg") {
			return FileType::PICTURE;
		}
		else {
			cerr << "Not A Valid File Type" << endl;
			return FileType::DOCUMENT;	// TODO: Should throw an exception
		}
	}

	// called whenever a path is set for the configuration file, just in case the file is empty
	void Configurable::populateFile() {

		// this is why the set of file types needs to be initialized before
		// the path can be set for the configuration file
		if (this->getPath() != "" && !this->getFileTypes().empty()) {
			fstream mFile;
			mFile.open(this->getPath(), ios::in);
			if (mFile.is_open()) {	// if the file exists and is not empty, there's no reason to populate it
				mFile.close();
			}
			else {
				ofstream strm(this->getPath());
				for (unordered_set<FileType>::iterator itr = this->fileTypes.begin(); itr != this->fileTypes.end(); ++itr) {
					strm << toString(*itr) << ": []" << endl;
				}
				strm.close();
				cout << "Configuration File Created: " << this->getPath() << endl;
			}
		}
	}

	unordered_set<FileType> Configurable::getFileTypes() {

		return this->fileTypes;
	}

	void Configurable::addFileType(FileType fileType) {

		this->fileTypes.insert(fileType);
	}

	string Configurable::getPath() {

		return this->path;
	}

	// calls populateFile() every time a new path is set
	void Configurable::setPath(string path) {

		this->path = path;
		this->populateFile();	// uses the set of FileTypes to populate an empty file
	}

	// updates THIS configuration file with the master_key that is passed, and returns the count for each updated FileType list
	unordered_map<FileType, int> Configurable::update(unordered_map<FileType, forward_list<string>> master) {

		// the 'int' value refers to the number of individual file changes associated with the given FileType
		unordered_map<FileType, int> updates;	
		fstream mFile;
		mFile.open(this->getPath(), ios::in);	// open the file for reading
		if (mFile.is_open()) {

			string line;
			while (getline(mFile, line)) {
				size_t pos = line.find_first_of(':');
				if (pos != string::npos) {	// if there is an array of files
					FileType type = toFileType(line.substr(0, pos));	// get the FileType associated with the array
					size_t begin = line.find_first_of('[');
					string elements = line.substr(begin, line.length() - begin);	// create a string to hold the file names
					size_t end = line.find_first_of(']');
					while (end == string::npos) {	// if the array spans more than one line
						getline(mFile, line);
						elements.append(" " + line);	// append each line to the string holding the file names// if the array spans more than one line
						end = line.find_first_of(']');
					}

					try {
						int count = 0;	// number of changes per FileType
						forward_list<string> list = master.at(type);	// the master_key list of files at a specific FileType index
						for (string val : list) {	// for each file in the list
							if (elements.find(val) == string::npos) {	// if THIS configuration file doesn't contain the file
								count++;	// increment the number of changes for this FileType
							}
						}
						updates.insert(make_pair(type, count));	// add a new key[FileType]-value[nChanges] pair to the list of updates
					}
					catch (const out_of_range& err) {}	// TODO: Throw again
				}
			}

			mFile.close();

			unordered_set<FileType> types = this->getFileTypes();

			ofstream strm(this->getPath());	// open THIS configuration file for writing
			
			// for each FileType in the master_key: output the list of files to THIS configuration file
			for (unordered_map<FileType, forward_list<string>>::iterator masterITR = master.begin(); masterITR != master.end(); ++masterITR) {

				if (types.find(masterITR->first) != types.end()) {

					forward_list<string> changed = masterITR->second;
					strm << FileHandler::toString(masterITR->first) << ": [";
					bool first = true;

					for (forward_list<string>::iterator innerITR = changed.begin(); innerITR != changed.end(); ++innerITR) {
						if (first) { strm << *innerITR; first = false; }
						else strm << ", " << *innerITR;
					}
					strm << "]" << endl;
				}
			}

			strm.close();
		}

		return updates;
	}

	// returns true if THIS configuration file contains the passed file name
	bool Configurable::contains(std::string fileName) {

		bool present = false;

		fstream mFile;
		mFile.open(this->getPath(), ios::in);
		if (mFile.is_open()) {
		
			string line;
			while (getline(mFile, line)) {

				size_t pos = line.find(fileName);
				if (pos != string::npos) {	// if the fileName is found, break and return true
					present = true;
					break;
				}
			}

			mFile.close();
		}

		// TODO: Throw if 'mFile' cannot open
		return present;
	}

	// returns a list of all files in the passed Configurable object's configuration file
	unordered_map<FileType, forward_list<string>> pleb_key(Configurable obj) {

		unordered_map<FileType, forward_list<string>> pleb_key;	// all the files in the configuration file
		fstream mFile;
		mFile.open(obj.getPath(), ios::in);	// open the configuration file for reading
		if (mFile.is_open()) {
			string line;
			while (getline(mFile, line)) {
				size_t pos = line.find_first_of(':');
				if (pos != string::npos) {	// if there is an array of files
					FileType type = toFileType(line.substr(0, pos));	// get the FileType associated with the array
					size_t begin = line.find_first_of('[');
					string elements = line.substr(begin, line.length() - begin);	// create a string to hold all the file names
					size_t end = line.find_first_of(']');
					while (end == string::npos) {	// if the array spans more than one line
						getline(mFile, line);
						elements.append(" " + line);	// append each line to the string holding the file names
						end = line.find_first_of(']');
					}

					// splits the string of elements into individual file names,
					// and returns a list of all those files
					auto parse = [](string elements) {

						elements = elements.substr(1, elements.length() - 2);	// cuts off the '[' and ']'
						forward_list<string> files;	// create a list of file names
						size_t end = elements.find_first_of(',');
						while ( end != string::npos) {	// while there is more than 1 file name in the string
							files.push_front(elements.substr(0, end));	// add the first file to the list of file names
							elements = elements.substr(end + 2, elements.length() - (end + 2));	// cut off the added file
							end = elements.find_first_of(',');
						}
						if (elements.length() > 4) {	// if there is still another file
							files.push_front(elements);	// add it to the list of file names
						}

						return files;
					};
					
					// for each FileType found in the cofiguration file:
					// add a new key[FileType]-value[list of file names] pair to the pleb_key
					pleb_key.insert(make_pair(type, parse(elements)));
				}
			}

			return pleb_key;
		}
		else {

			// TODO: Should Throw an error here, because the file couldn't open
			return pleb_key;
		}
	}

	// returns a combined list of two pleb_keys; used to update configuration files
	unordered_map<FileType, forward_list<std::string>> master_key(unordered_map<FileType, forward_list<string>> pleb_key1,
		unordered_map<FileType, forward_list<string>> pleb_key2) {

		unordered_map<FileType, forward_list<string>> master_key;	// full list of files in both configuration files

	
		// add all the files from the first pleb_key to the master_key
		for (unordered_map<FileType, forward_list<string>>::iterator outerITR = pleb_key2.begin(); outerITR != pleb_key2.end(); ++outerITR) {
			for (forward_list<string>::iterator innerITR = outerITR->second.begin(); innerITR != outerITR->second.end(); ++innerITR) {
				master_key[outerITR->first].push_front(*innerITR);
			}
		}
		
		// add all the files from the second pleb_key to the master_key
		for (unordered_map<FileType, forward_list<string>>::iterator outerITR = pleb_key1.begin(); outerITR != pleb_key1.end(); ++outerITR) {
			for (forward_list<string>::iterator innerITR = outerITR->second.begin(); innerITR != outerITR->second.end(); ++innerITR) {
				master_key[outerITR->first].push_front(*innerITR);
			}
		}

		// sort each list in the master_key, and remove duplicate file names
		for (unordered_map<FileType, forward_list<string>>::iterator itr = master_key.begin(); itr != master_key.end(); ++itr) {
			itr->second.sort();
			itr->second.unique();	
		}

		return master_key;
	}

	// returns a list generated from a master_key, pleb_key, and single FileType; used when a user only wants one FileType to update (e.g. MUSIC XOR PHOTOS)
	unordered_map<FileType, forward_list<string>> diff_key(unordered_map<FileType, forward_list<string>> master, unordered_map<FileType, forward_list<string>> pleb_key, FileHandler::FileType type) {

		// replace all the file names in the passed pleb_key with the list of file
		// names in the master_key associated with a given FileType
		pleb_key[type] = master[type];	
		return pleb_key;
	}
}