#ifndef PRINTER_H
#define PRINTER_H

#include "Device.h"

class Printer : public Device, public Operations::Scannable, public Operations::Printable {

public:

	Printer();

	void display();

	void scan(std::unordered_map<FileHandler::FileType, int> updates);

	void print(std::unordered_map<FileHandler::FileType, int> updates);
};

#endif