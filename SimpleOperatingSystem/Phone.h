#ifndef PHONE_H
#define PHONE_H

#include "Device.h"

class Phone : public Device, public Operations::Syncable {

private:

	static int count;

public:

	Phone();
	Phone(string path);

	void display();

	void sync(unordered_map<FileHandler::FileType, int> updates);
};

#endif