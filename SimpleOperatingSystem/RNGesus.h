#ifndef RNGESUS_H
#define RNGESUS_H

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <ctime>
#include "FileHandler.h"

/*
* RNGesus is used to simulate the time it takes to perform a certain action
*
* Any Device that implements one of the Operations interfaces has access to this namespace.
*/

namespace RNGesus {

	void showProgress(int percentage, std::string other);	// print the progress to standard output

	// initiate the simulation of various time dependent behaviours
	void initialize(int count, FileHandler::FileType type, std::string other);
}

#endif