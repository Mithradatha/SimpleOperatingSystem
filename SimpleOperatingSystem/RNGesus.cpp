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

		time_t begin = time(0);
		time_t end = begin + distribution(generator) * count;
		time_t last = 0;
		time_t current = 0;
		end = end - begin;
		double increment = end / 10.0;
		
		showProgress(0, other);

		while (current < end) {

			if ((current - last) > increment) {

				showProgress(((current / (double)end) * 100), other);
				last = current;
			}

			current = time(0) - begin;
		}

		showProgress(100, other);
	}
}