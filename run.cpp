#include "smeagle/smeagle.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
	if(argc != 2) {
		std::cerr << "Usage: " << argv[0] << " file\n";
		return -1;
	}

	try {
		std::cout << smeagle::parse(argv[1]).to_asp();
	} catch(std::exception &ex) {
		std::cerr << ex.what() << '\n';
	}
}
