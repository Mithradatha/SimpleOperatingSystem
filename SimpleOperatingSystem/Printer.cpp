#include "Printer.h"

Printer::Printer() {
	
	this->setConnection(false);
}

void Printer::display() {

	cout << "PRINTER" << endl;
}

void Printer::scan(std::string fileName, FileHandler::FileType type) {

	RNGesus::initialize(1, type, "Scanning... ");
}

void Printer::print(std::string fileName, FileHandler::FileType type) {

	RNGesus::initialize(1, type, "Printing... ");
}