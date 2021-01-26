#include "stdafx.h"
#include "mem.h"
#include <assert.h>

// read process memory
//template <typename T>
//inline bool
//skeet2::ProcessMemoryManager::RPM(DWORD dwAddress,
//								  T& returnValue)
//{
//	return ReadProcessMemory(m_hProcess,
//							 reinterpret_cast<LPVOID>( dwAddress ),
//							 (LPVOID) returnValue,
//							 sizeof(returnValue),
//							 NULL)
//		== true;
//}

// write process memory
//template <typename T>
//inline bool
//skeet2::ProcessMemoryManager::WPM(DWORD dwAddress,
//								  const T& returnValue)
//{
//	return WriteProcessMemory(m_hProcess,
//							  reinterpret_cast<LPVOID>( dwAddress ),
//							  returnValue, sizeof(T),
//							  NULL)
//		== true;
//}

// constructor that defines member variables and attaches to specified program
skeet2::ProcessMemoryManager::ProcessMemoryManager(const std::string& sProcessName)
{
	m_hProcess = INVALID_HANDLE_VALUE;
	m_dwProcessId = 0;
	m_Modules.clear();

	#if CONSUMER_BUILD == 0
	assert(Attach(sProcessName) && "Could not find CSGO");
	#else
	std::cout << "ERROR GETTING CSGO (POSSIBLY NOT RUNNING)" << std::endl;
	#endif
}

// getting module and program info
bool skeet2::ProcessMemoryManager::Attach(const std::string& sProcessName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); // create a snapshot of processes in memory
	PROCESSENTRY32 procEntry; // process INFORMATION!
	procEntry.dwSize = sizeof(PROCESSENTRY32);

	if (hSnapshot == INVALID_HANDLE_VALUE) // check if snapshot is invalid
		return false; // function unsuccesful

	if (Process32First(hSnapshot, &procEntry)) // take the info from the first process from the snapshot and store it to procEntry
	{
		if (!std::wcscmp(procEntry.szExeFile,
						 std::wstring(sProcessName.begin(),
									  sProcessName.end()).c_str())
			)
		{
			CloseHandle(hSnapshot);
			m_hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ, FALSE, procEntry.th32ProcessID);
			m_dwProcessId = procEntry.th32ProcessID;
			m_procEntry = procEntry;
			return true; // function successful
		}
	}
	else
	{
		CloseHandle(hSnapshot);
		return false; // Process32First call failed
	}

	while (Process32Next(hSnapshot, &procEntry))
	{
		if (!std::wcscmp(procEntry.szExeFile,
						 std::wstring(sProcessName.begin(),
									  sProcessName.end()).c_str())
			)
		{
			m_hProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ, FALSE, procEntry.th32ProcessID);
			m_dwProcessId = procEntry.th32ProcessID;
			m_procEntry = procEntry;
			return true;
		}
	}
	CloseHandle(hSnapshot);
	return false;
}
bool skeet2::ProcessMemoryManager::GrabModule(const std::string& sModuleName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return false;

	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &moduleEntry))
	{
		if (!std::wcscmp(moduleEntry.szModule, std::wstring(sModuleName.begin(), sModuleName.end()).c_str()))
		{
			// module found!
			CloseHandle(hSnapshot);
			m_Modules.push_back(moduleEntry);
			return true;
		}
	}
	else
	{
		CloseHandle(hSnapshot);
		return true;
	}

	while (Module32Next(hSnapshot, &moduleEntry))
	{
		if (!std::wcscmp(moduleEntry.szModule, std::wstring(sModuleName.begin(), sModuleName.end()).c_str()))
		{
			CloseHandle(hSnapshot);
			m_Modules.push_back(moduleEntry);
			return true;
		}
	}

	CloseHandle(hSnapshot);
	return false;
}

// return functions
DWORD skeet2::ProcessMemoryManager::GetProcId()
{
	return m_dwProcessId;
}
HANDLE skeet2::ProcessMemoryManager::GetHandle()
{
	return m_hProcess;
}
std::vector<MODULEENTRY32> skeet2::ProcessMemoryManager::GetModules()
{
	return m_Modules;
}