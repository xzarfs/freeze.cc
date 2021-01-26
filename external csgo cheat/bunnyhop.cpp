#include "stdafx.h"
#include "bunnyhop.h"
#include "cheat_manager.h"
#include "definitions.h"

namespace skeet2
{
	Bunnyhop::Bunnyhop(ProcessMemoryManager& memMgr)
	{
		m_DisplayName = "Bunnyhop";
		m_Name = "bunnyhop";
		m_Version = 1.0f;
		m_options["chance"] = 1;
		m_pMemMgr = &memMgr;
	}

	bool Bunnyhop::Load()
	{
		if (!m_pMemMgr->GrabModule("client.dll"))
		{
			std::cout << "Client module not found!" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
			return false;
		}
		for (UINT i = 0; i < m_pMemMgr->GetModules().size(); i++) // at this point we have client.dll in module vector
		{
			if (!wcscmp(m_pMemMgr->GetModules()[i].szModule, L"client.dll")) // if the module currently searching is client.dll (wscmp returns non-zero when different wstrings)
			{
				m_dwClientBaseAddress = (DWORD) m_pMemMgr->GetModules()[i].modBaseAddr; // modBaseAddr is a pointer to module base
				break;
			}
		}
		m_pMemMgr->RPM<DWORD>(m_dwClientBaseAddress + signatures::dwLocalPlayer, m_dwLocalPlayerAddress); // dereference local player FROM memory by reading from [Client.dll + localplayer offset]
		/*int x = 0;
				ReadProcessMemory(m_pMemMgr->GetHandle(),
						  (LPCVOID) (0x2df20000),
						  (LPVOID) &x,
						  sizeof(int),
						  NULL);*/
		DWORD lastError = GetLastError();
		std::cout << lastError << std::endl;
		return true; // function was successful
	}

	void Bunnyhop::MainLoop() // start the main loop for bunnyhop (mainly checks input etc)
	{
		DWORD flagsAddr = static_cast<DWORD>( ( m_dwLocalPlayerAddress + 0x104 ) );
		DWORD isOnGround;
		DWORD* jumpKeyAddr = 0;
		m_pMemMgr->RPM(m_dwClientBaseAddress + dfn::DW_FORCEJUMP_POINTER_OFFSET, jumpKeyAddr);
		jumpKeyAddr += ( dfn::DW_FORCEJUMP_OFFSET / sizeof(DWORD) );

		DWORD lastError = GetLastError();
		std::cout << lastError << std::endl;
		while (TRUE)
		{
			if (( GetAsyncKeyState(m_iVKey) & 0x8000 ))
			{ // get the keys state bit flag (using bit mask)
				m_pMemMgr->RPM((DWORD) flagsAddr, isOnGround);
				if (( isOnGround & dfn::FL_ONGROUND ))
				{
					m_pMemMgr->WPM(jumpKeyAddr, dfn::FORCE_JUMP);
					DWORD lastError = GetLastError();
					std::cout << lastError << std::endl;
				}
			}
		}
	}
}