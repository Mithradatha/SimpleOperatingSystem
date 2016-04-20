#include "Printer.h"

Printer::Printer() {
	
	this->setConnection(false);
}

void Printer::display() {

	cout << "PRINTER" << endl;
}

void Printer::scan(std::unordered_map<FileHandler::FileType, int> updates) {


}

void Printer::print(std::unordered_map<FileHandler::FileType, int> updates) {


}