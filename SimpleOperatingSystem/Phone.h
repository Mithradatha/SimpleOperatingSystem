#ifndef PHONE_H
#define PHONE_H

#include "Device.h"

class Phone : public Device, public Operations::Syncable {

public:

	Phone(string path);

	void display();

	void sync(unordered_map<FileHandler::FileType, int> updates);
};

#endif