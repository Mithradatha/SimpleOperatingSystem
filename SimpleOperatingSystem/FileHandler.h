#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <forward_list>
#include <unordered_map>
#include <unordered_set>

/*
* FileHandler has three distinct parts:
* 
* 1) Enumeration: FileType { DOCUMENT, PICTURE, MUSIC}
*		1. Operations on the enumeration:
*			a. toString
*			b. toExtension
*			c. toFileType
*
* 2) Concrete Class: Configurable
*		1. Data:
*			a. path
*			b. set of FileTypes (Refer to (1) 'FileType' Enum)
*		2. Operations:
*			a. Getters/Setters:
*				i) path
*			   ii) set of FileTypes
*			b. populateFile [private access]
*			c. update
*			d. contains
*
* 3) File Handling Operations:
*		1. pleb_key
*		2. master_key
*		3. diff_key
*/


// TODO: Create 'typedef' for std::unordered_map<FileType, std::forward_list<std::string>>

namespace FileHandler {

	enum class FileType { DOCUMENT, PICTURE, MUSIC };	// new file types must be added here, and implemented in the operations below
	std::string toString(FileType fileType);	// converts from an Enum 'FileType' to an equivalent string: DOCUMENT -> "DOCUMENT"
	std::string toExtension(FileType fileType);	// converts from an Enum 'FileType' to the associated file extension: DOCUMENT -> ".doc"
	FileType toFileType(std::string str);	// converts from a string to the associated Enum 'FileType': ".doc"/"DOCUMENT" -> DOCUMENT

	/*
	* Configurable should be inherited by any class that USES a configuration file (e.g. "Phone", "OperatingSystem")
	*
	* *IMPORTANT*: Any class that inherits from Configurable MUST initialize the members in this order:
	*		1) FOR_EACH ( allowed file type : Configurable Derived Class ) { addFileType( allowed file type); }
	*		2) Configurable Derived Class -> setPath ( path );
	*
	* TODO: Make this^ unnecessary
	* TODO: Configurable should not be a concrete class
	* TODO: Replace 'path' with file handle
	* TODO: Create a separate function for repeated file traversal methods
	*/
	class Configurable {

	private:

		std::string path;	// path to the configuration file associated with the derived class
		std::unordered_set<FileType> fileTypes;	// the set of allowed file types within the configuration file

		// if the configuration file is empty, then populate it with empty file type containers:
		// DOCUMENT: []
		// PICTURE: []
		// ...
		void populateFile();

	public:

		std::unordered_set<FileType> getFileTypes();	// return the allowed file types
		void addFileType(FileType fileType);	// add an individual allowed file type to the set of allowed file types
		
		std::string getPath();	// return the configuration file path
		void setPath(std::string path);	// set the configuration file path

		// write the passed master_key to THIS configuration file,
		// and return the number of updates associated with each file type
		std::unordered_map<FileType, int> update(std::unordered_map<FileType, std::forward_list<std::string>> master);
		
		bool contains(std::string fileName);	// does THIS configuration file contain the file name?
	};

	// return a pleb_key representing all the files in the passed Configurable object's configuration file
	std::unordered_map<FileType, std::forward_list<std::string>> pleb_key(Configurable obj);

	// return a master_key representing the combination of two pleb_key's files
	std::unordered_map<FileType, std::forward_list<std::string>> master_key(std::unordered_map<FileType, std::forward_list<std::string>> pleb_key1, std::unordered_map<FileType, std::forward_list<std::string>> pleb_key2);

	// return a master_key representing the combination of a pleb_key and a single list of files, associated with the passed file type, from the master_key
	std::unordered_map<FileType, std::forward_list<std::string>> diff_key(std::unordered_map<FileType, std::forward_list<std::string>> master, std::unordered_map<FileType, std::forward_list<std::string>> pleb_key, FileHandler::FileType type);


											
	
											/* OPERATING SYSTEM CONFIGURATION FILE */

				/* BEFORE UNINITIALIZATION */										/* AFTER UNINTIALIZATION */

	// SystemConfigFile.txt												// SystemConfigFile.txt
	//**********************************************//					//**********************************************//
	//MUSIC: [reggae.mp3]							//					//MUSIC: [reggae.mp3]							//
	//PICTURE: [Thor.jpg, Apocalypse.jpg]			//					//PICTURE: [Thor.jpg, Apocalypse.jpg]			//
	//DOCUMENT: [Lab_1.doc, Lab_2.doc, bonus.doc]	//					//DOCUMENT: [Lab_1.doc, Lab_2.doc, bonus.doc]	//
	//												//					///*											//
	//**********************************************//					//{phone}(PhoneConfigFile.txt)					//		
																		//{printer}										//
																		//*/											//
																		//												//
																		//**********************************************//
			/* PHONE CONFIGURATION FILE */

	// PhoneConfigFile.txt
	//**********************************************//
	//MUSIC: [punk.mp3]								//
	//PICTURE: [Beyonder.jpg, Apocalypse.jpg]		//
	//												//
	//**********************************************//
}

#endif
