#include <windows.h>

#include <iostream>

#include "gmod.hpp"

LPVOID allocate_memory(HANDLE process_handle, LPCVOID buffer, size_t size) {
	LPVOID allocated_memory = VirtualAllocEx(process_handle, NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (allocated_memory == NULL) {
		return NULL;
	}

	if (WriteProcessMemory(process_handle, allocated_memory, buffer, size, NULL) == 0) {
		VirtualFreeEx(process_handle, allocated_memory, 0, MEM_RELEASE);

		return NULL;
	}

	return allocated_memory;
}

int main() {
	gmod::get_gmod();

	// file stuff
	char module_name[] = "dll.dll";
	char module_full_name[MAX_PATH] = { };

	if (GetFileAttributes(module_name) == 0xFFFFFFFF)
		return 1; // yep this is the "error handling"

	GetFullPathNameA(module_name, MAX_PATH, module_full_name, NULL);

	// memory stuff
	HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, false, gmod::process_id);

	if (process_handle == NULL)
		return 2;

	LPVOID allocated_memory = allocate_memory(process_handle, module_full_name, sizeof(module_full_name));
	if (allocated_memory == NULL) {
		CloseHandle(process_handle);

		return 3;
	}

	HANDLE thread_handle = CreateRemoteThread(
		process_handle,
		NULL,
		0,
		reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA")), // should be fine
		allocated_memory,
		0,
		NULL
	);

	// exitting stuff
	if (thread_handle != NULL) {
		WaitForSingleObject(thread_handle, INFINITE);
		CloseHandle(thread_handle);
	}
	else {
		return 4;
	}

	VirtualFreeEx(process_handle, allocated_memory, 0, MEM_RELEASE);

	CloseHandle(process_handle);

	return 0;
}