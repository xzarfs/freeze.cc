#pragma once
#include "stdafx.h"

namespace skeet2 //skeety 2.0
{
	class ProcessMemoryManager // wpm and rpm abstracted
	{
	private:
		PROCESSENTRY32 m_procEntry;				// process info
		std::vector<MODULEENTRY32> m_Modules;	// list of modules in process (dll and processes)
		DWORD m_dwProcessId;					// current process id
		HANDLE m_hProcess;						// current process
	public:
		/*template <typename T> inline bool RPM(DWORD dwAddress,
										   T& returnValue);*/
		template <typename T>
		inline bool
			RPM(DWORD dwAddress,
				T& returnValue
				)
		{
			return ReadProcessMemory(m_hProcess,
									 (LPCVOID) dwAddress,
									 (LPVOID) &returnValue, // !!!!FUCK YES!!11!11! FINALLY SOLVED THIS (NOT WORKING BCS WAS NOT TAKING THIS ARGUMENT AS REFERENCE!!!!!!!!)
									 sizeof(T),
									 NULL);
		}

		template <typename T>
		inline bool
			WPM(DWORD* dwAddress, // must point to the desired address
				T& returnValue// pointer to what you wanna write
				)
		{
			return WriteProcessMemory(m_hProcess,
									  (LPVOID) dwAddress,
									  (LPVOID) &returnValue,
									  sizeof(T),
									  NULL);
		}

		ProcessMemoryManager(const std::string& sProcessName = "csgo.exe"); // constructor

		bool Attach(const std::string& sProcessName); // attach class to a program (called in constructor)
		bool GrabModule(const std::string& sModuleName); //get a module by name and put it in m_Modules

		// getter functions
		HANDLE GetHandle();
		DWORD GetProcId();
		std::vector<MODULEENTRY32> GetModules();
	};
}

