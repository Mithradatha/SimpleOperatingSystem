#include "RNGesus.h"

namespace RNGesus {
	
	void showProgress(int percentage, std::string other) {

		std::cout << other << percentage << "%" << std::endl;
	}

	void initialize(int count, FileHandler::FileType type, std::string other) {

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);

		int min;
		int max;

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

		int current = time(0);
		int last = current;
		int total = current + distribution(generator) * count;
		int increment = total / 5;
		
		showProgress(0, other);

		while (current < total) {

			if ((current - last) > increment) {

				showProgress(((current / total) * 100), other);
				last = current;
			}

			current = time(0);
		}

		showProgress(100, other);
	}
}