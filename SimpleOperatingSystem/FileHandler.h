#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>

namespace FileHandler {

	enum class FileType { DOCUMENT, PICTURE, MUSIC };
	std::string toString(FileType fileType);
	std::string toExtension(FileType fileType);
	FileType toFileType(std::string str);


	class Configurable {

	private:

		std::string path;
		std::unordered_set<FileType> fileTypes;

		void populateFile();

	public:

		std::unordered_set<FileType> getFileTypes();
		void addFileType(FileType fileType);
		
		std::string getPath();
		void setPath(std::string path);

		std::unordered_map<FileType, int> update(std::unordered_map<FileType, std::forward_list<std::string>> master);
		bool contains(std::string fileName);
	};

	std::unordered_map<FileType, std::forward_list<std::string>> pleb_key(Configurable obj);
	std::unordered_map<FileType, std::forward_list<std::string>> master_key(std::unordered_map<FileType, std::forward_list<std::string>> pleb_key1, std::unordered_map<FileType, std::forward_list<std::string>> pleb_key2);
	std::unordered_map<FileType, std::forward_list<std::string>> diff_key(std::unordered_map<FileType, std::forward_list<std::string>> master, std::unordered_map<FileType, std::forward_list<std::string>> pleb_key, FileHandler::FileType type);
}

#endif