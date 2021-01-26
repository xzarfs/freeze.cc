#pragma once
#include "stdafx.h"
#include "cheat_manager.h"
#include "mem.h"

namespace skeet2
{
	class Bunnyhop : public CheatMeta
	{
	protected:
		LPVOID m_pdwForceJump = 0;
		ProcessMemoryManager* m_pMemMgr; // memory managment class pointer
		DWORD m_dwClientBaseAddress = 0; // address for client base
		DWORD m_dwLocalPlayerAddress = 0;
		BYTE m_Flags;
	public:
		int m_iVKey = VK_SPACE; // key for activating bhop
		Bunnyhop(ProcessMemoryManager& memMgr); // CONSTRUCTOR that defines cheat meta
		bool Load(); // enables the cheat
		void MainLoop();
		// TODO: refresh function
	};
}

