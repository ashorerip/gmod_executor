#pragma once

#include <windows.h>

void* get_interface(const char* module_name, const char* interface_name) {
	const auto module_handle = GetModuleHandle(module_name);

	if (module_handle == nullptr)
		return nullptr;

	const auto process_address = GetProcAddress(module_handle, "CreateInterface");

	if (process_address == nullptr)
		return nullptr;

	using CreateInterface_t = void* (*)(const char*, int*);
	auto CreateInterface = reinterpret_cast<CreateInterface_t>(process_address);

	return CreateInterface(interface_name, 0);
}