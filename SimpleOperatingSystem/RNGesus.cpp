#include "RNGesus.h"

namespace RNGesus {
	
	void showProgress(int percentage, std::string other) {

		std::cout << other << percentage << "%" << std::endl;	// "Coding... 80%"
	}

	void initialize(int count, FileHandler::FileType type, std::string other) {

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);	// create a generator based on a "random" seed

		int min;	// generator min range
		int max;	// generator max range

		// DOCUMENTS < MUSIC < PICTURES

		switch (type) {

		case FileHandler::FileType::DOCUMENT:

			min = 1;
			max = 5;
			break;

		case FileHandler::FileType::MUSIC:

			min = 6;
			max = 10;
			break;

		case FileHandler::FileType::PICTURE:

			min = 11;
			max = 15;
			break;
		}

		std::uniform_int_distribution<int> distribution(min, max);

		time_t begin = time(0);	// get the current system time

		// the total time = current time + (random number based on FileType range * nFilesChanged)
		time_t end = begin + distribution(generator) * count;
		
		time_t last = 0;	// time since the last standard output
		time_t current = 0;	// current system time
		end = end - begin;
		double increment = end / 10.0;	// allowed time between each standard output
		
		showProgress(0, other);	// 0%

		while (current < end) {	// while action still performing

			if ((current - last) > increment) {	// if difference meets increment constraint

				showProgress(((current / (double)end) * 100), other);	// standard output based on percentage completed
				last = current;
			}

			current = time(0) - begin;
		}

		showProgress(100, other);	// 100%
	}
}
