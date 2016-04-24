#ifndef PRINTER_H
#define PRINTER_H

#include "Device.h"

// 'Is A' Device: Implements 'display()', inherits 'connected'
class Printer : public Device, public Operations::Scannable, public Operations::Printable {

public:

	Printer();

	void display();	// display the device to standard output

	void scan(std::string fileName, FileHandler::FileType type);	// simulate file scanning

	void print(std::string fileName, FileHandler::FileType type);	// simulate file printing
};

#endif