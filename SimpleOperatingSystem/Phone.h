#ifndef PHONE_H
#define PHONE_H

#include "Device.h"

// 'Is A' Device: Implements 'display()', inherits 'connected'
// *IMPORTANT*: Refer to Operations::Syncable for Phone initialization
class Phone : public Device, public Operations::Syncable {

public:

	Phone(string path);

	void display();	// display the device to standard output

	void sync(unordered_map<FileHandler::FileType, int> updates);	// simulate file syncrhonization
};

#endif