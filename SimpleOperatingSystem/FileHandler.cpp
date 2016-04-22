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
			return "";
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
			return "";
		}
	}

	FileType toFileType(string str) {

		if (str == "DOCUMENT" || str == ".doc") {
			return FileType::DOCUMENT;
		}
		else if (str == "MUSIC" || str == ".mp3") {
			return FileType::MUSIC;
		}
		else if (str == "PICTURE" || str == ".jpg") {
			return FileType::PICTURE;
		}
		else {
			cerr << "Not A Valid File Type" << endl;
		}
	}

	void Configurable::populateFile() {

		if (this->getPath() != "" && !this->getFileTypes().empty()) {
			fstream mFile;
			mFile.open(this->getPath(), ios::in);
			if (mFile.is_open()) {
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

	void Configurable::setPath(string path) {

		this->path = path;
		this->populateFile();
	}

	unordered_map<FileType, int> Configurable::update(unordered_map<FileType, forward_list<string>> master) {

		unordered_map<FileType, int> updates;

		fstream mFile;
		mFile.open(this->getPath(), ios::in);
		if (mFile.is_open()) {

			string line;
			while (getline(mFile, line)) {
				size_t pos = line.find_first_of(':');
				if (pos != string::npos) {
					FileType type = toFileType(line.substr(0, pos));
					size_t begin = line.find_first_of('[');
					string elements = line.substr(begin, line.length() - begin);
					while (size_t end = line.find_first_of(']') == string::npos) {
						getline(mFile, line);
						elements.append(" " + line);
					}

					try {
						int count = 0;
						forward_list<string> list = master.at(type);
						for (string val : list) {
							if (elements.find(val) == string::npos) {
								count++;
							}
						}
						updates.insert(make_pair(type, count));
					}
					catch (const out_of_range& err) {}
				}
			}

			mFile.close();

			unordered_set<FileType> types = this->getFileTypes();

			ofstream strm(this->getPath());
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

	bool Configurable::contains(std::string fileName) {

		bool present = false;

		fstream mFile;
		mFile.open(this->getPath(), ios::in);
		if (mFile.is_open()) {
		
			string line;
			while (getline(mFile, line)) {

				size_t pos = line.find(fileName);
				if (pos != string::npos) {
					present = true;
					break;
				}
			}

			mFile.close();
		}

		return present;
	}

	unordered_map<FileType, forward_list<string>> pleb_key(Configurable obj) {

		unordered_map<FileType, forward_list<string>> pleb_key;	// all the files in a path
		fstream mFile;
		mFile.open(obj.getPath(), ios::in);
		if (mFile.is_open()) {
			string line;
			while (getline(mFile, line)) {
				size_t pos = line.find_first_of(':');
				if (pos != string::npos) {
					FileType type = toFileType(line.substr(0, pos));
					size_t begin = line.find_first_of('[');
					string elements = line.substr(begin, line.length() - begin);
					while (size_t end = line.find_first_of(']') == string::npos) {
						getline(mFile, line);
						elements.append(" " + line);
					}

					auto parse = [](string elements) {

						elements = elements.substr(1, elements.length() - 2);
						forward_list<string> files;
						size_t end = elements.find_first_of(',');
						while ( end != string::npos) {
							files.push_front(elements.substr(0, end));
							elements = elements.substr(end + 2, elements.length() - (end + 2));
							end = elements.find_first_of(',');
						}
						if (elements.length() > 4) {
							files.push_front(elements);
						}

						return files;
					};
					
					pleb_key.insert(make_pair(type, parse(elements)));
				}
			}

			return pleb_key;
		}
		else {

			return pleb_key;
			// Error Opening File
		}
	}

	unordered_map<FileType, forward_list<std::string>> master_key(unordered_map<FileType, forward_list<string>> pleb_key1,
		unordered_map<FileType, forward_list<string>> pleb_key2) {

		unordered_map<FileType, forward_list<string>> master_key;

	
		for (unordered_map<FileType, forward_list<string>>::iterator outerITR = pleb_key2.begin(); outerITR != pleb_key2.end(); ++outerITR) {
			for (forward_list<string>::iterator innerITR = outerITR->second.begin(); innerITR != outerITR->second.end(); ++innerITR) {
				master_key[outerITR->first].push_front(*innerITR);
			}
		}
		
		for (unordered_map<FileType, forward_list<string>>::iterator outerITR = pleb_key1.begin(); outerITR != pleb_key1.end(); ++outerITR) {
			for (forward_list<string>::iterator innerITR = outerITR->second.begin(); innerITR != outerITR->second.end(); ++innerITR) {
				master_key[outerITR->first].push_front(*innerITR);
			}
		}

		for (unordered_map<FileType, forward_list<string>>::iterator itr = master_key.begin(); itr != master_key.end(); ++itr) {
			itr->second.sort();
			itr->second.unique();	
		}

		return master_key;
	}

	unordered_map<FileType, forward_list<string>> diff_key(unordered_map<FileType, forward_list<string>> master, unordered_map<FileType, forward_list<string>> pleb_key, FileHandler::FileType type) {

		pleb_key[type] = master[type];
		return pleb_key;
	}
}