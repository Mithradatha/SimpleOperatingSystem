#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include "FileHandler.h"

/**
* Functional interface container for various device behaviours
*/

namespace Operations {

	class Syncable : public FileHandler::Configurable {

	public:

		virtual void sync(std::unordered_map<FileHandler::FileType, int> updates) = 0;
	};

	class Scannable {

	public:

		virtual void scan(std::unordered_map<FileHandler::FileType, int> updates) = 0;
	};
	
	class Printable {

	public:

		virtual void print(std::unordered_map<FileHandler::FileType, int> updates) = 0;
	};
}

#endif