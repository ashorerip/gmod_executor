#include <windows.h>
#include <tlhelp32.h>

namespace gmod {
	bool get_gmod();
	DWORD process_id = 0;

	bool injected = false;
}

bool gmod::get_gmod() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == NULL || snapshot == INVALID_HANDLE_VALUE) {
		return 1;
	}

	PROCESSENTRY32 process_entry;
	process_entry.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snapshot, &process_entry)) {
		CloseHandle(snapshot);

		return 1;
	}

	do {
		if (strncmp(process_entry.szExeFile, "gmod.exe", strlen(process_entry.szExeFile)) == 0) {
			gmod::process_id = process_entry.th32ProcessID;

			return true;
		}
	} while (Process32Next(snapshot, &process_entry));

	CloseHandle(snapshot);

	return false;
}