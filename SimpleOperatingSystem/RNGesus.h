#ifndef RNGESUS_H
#define RNGESUS_H

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <ctime>
#include "FileHandler.h"

namespace RNGesus {

	void showProgress(int percentage, std::string other);

	void initialize(int count, FileHandler::FileType type, std::string other);
}

#endif