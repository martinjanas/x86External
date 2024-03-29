#include "MemoryManager.h"
#include <TlHelp32.h>
#include <iostream>
#include <iomanip>

namespace p
{
	uintptr_t get_process(const wchar_t* proc)
	{
		HANDLE hProcessId = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		uintptr_t process;
		PROCESSENTRY32 pEntry;
		pEntry.dwSize = sizeof(pEntry);

		do
		{
			if (!_wcsicmp(pEntry.szExeFile, proc))
			{
				process = pEntry.th32ProcessID;
				CloseHandle(hProcessId);
				handle = OpenProcess(PROCESS_ALL_ACCESS, false, process);
			}

		} while (Process32Next(hProcessId, &pEntry));
		return process;
	}

	uintptr_t get_module(uintptr_t proc_id, const wchar_t* module_name)
	{
		HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);
		MODULEENTRY32 mEntry;
		mEntry.dwSize = sizeof(mEntry);

		do
		{
			if (!_wcsicmp(mEntry.szModule, module_name))
			{
				CloseHandle(hModule);
				return (uintptr_t)mEntry.hModule;
			}
		} while (Module32Next(hModule, &mEntry));
		return 0;
	}
}
