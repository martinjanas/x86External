#pragma once
#include <Windows.h>

namespace p
{
	uintptr_t get_process(const wchar_t* proc);
	uintptr_t get_module(uintptr_t proc_id, const wchar_t* module_name);

	inline HANDLE handle;

	template <class val>
	val read(uintptr_t addr)
	{
		val x;
		ReadProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}

	template <class val>
	val write(uintptr_t addr, val x)
	{
		WriteProcessMemory(handle, (LPBYTE*)addr, &x, sizeof(x), NULL);
		return x;
	}
}