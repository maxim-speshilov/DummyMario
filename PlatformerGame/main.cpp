#include "Application.h"
#include "SettingsIdentifiers.h"
#include <iostream>
#include "Utility.h"
#include <SFML/Graphics.hpp>

int main() {	
	try {
		Application app;
		app.run();
	} catch (std::exception& e) {
		std::cout << "\nException: " << e.what() << std::endl;
	}

	return 0;
}