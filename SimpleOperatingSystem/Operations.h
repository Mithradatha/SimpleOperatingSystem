#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>
#include "FileHandler.h"
#include "RNGesus.h"

/**
* Functional interface namespace for various device behaviours
*
* #includes 'RNGesus' for simulating the behaviors
* Note: RNGesus::initialize(int, FileType, string)
*/

namespace Operations {

	// *IMPORTANT*: Syncable derives from Configurable, so all Devices that implement Syncable
	//				also inherit the Configurable member variables/operations
	//				(Most Notably, a 'path' refering to the configuration file.
	//				This means that the addFileType(), then setPath() sequence
	//				must be followed in all Devices that implement Syncable.
	//	
	// Refer to FileHandler::Configurable for more information.
	class Syncable : public FileHandler::Configurable {

	public:

		// @updates = list of FileTypes and associated number of changes
		virtual void sync(std::unordered_map<FileHandler::FileType, int> updates) = 0;
	};

	class Scannable {

	public:

		// @fileName = name of file to scan into operating system
		// @type = FileType of file to scan into operating system
		virtual void scan(std::string fileName, FileHandler::FileType type) = 0;
	};
	
	class Printable {

	public:

		// @fileName = name of file to print out to device
		// @type = FileType of file to print out to device
		virtual void print(std::string fileName, FileHandler::FileType type) = 0;
	};
}

#endif