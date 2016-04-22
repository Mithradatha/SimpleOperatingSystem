#ifndef PRINTER_H
#define PRINTER_H

#include "Device.h"

class Printer : public Device, public Operations::Scannable, public Operations::Printable {

public:

	Printer();

	void display();

	void scan(std::string fileName, FileHandler::FileType type);

	void print(std::string fileName, FileHandler::FileType type);
};

#endif