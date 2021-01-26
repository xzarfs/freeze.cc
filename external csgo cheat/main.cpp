#include "stdafx.h"
#include "mem.h"
#include "cheat_manager.h"
#include "bunnyhop.h"

#define CONSUMER_BUILD 0		// if defined all asserts and exceptions will be removed
#define CONSOLE_ACTIVE_BUILD 0	// if defined a windows console (active for commands and logs debug messages) will be enabled

int main()
{
	skeet2::ProcessMemoryManager* pMemMgr = new skeet2::ProcessMemoryManager("csgo.exe"); // dynamically instances a memory managment class and attaches it to csgo
	skeet2::Bunnyhop bunnyhop(*pMemMgr); // dereferenced pointer to bhop

	if (bunnyhop.Load())
	{
		std::cout << "BHOP ENABLED\n" << std::endl;
		bunnyhop.MainLoop();
	}

	// IMPLEMENT MENU 
	std::cin.get();
}