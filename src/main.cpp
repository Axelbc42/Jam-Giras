#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include <exception>
#include <iostream>
#include <string>
#include "Game.h"

int main(int, char**) {

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try {
		if (!Game::Init()) {
			throw std::string("Error initializing Game");
		}

		Game::Instance()->initGame();
		Game::Instance()->start();

		Game::Release();

	}
	catch (const std::string& e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	}
	catch (const char* e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	}
	catch (const std::exception& e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Caught an exception of unknown type ..." << std::endl;
	}

	return 0;
}