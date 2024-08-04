constexpr auto PORT = 8012;

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <thread>

// lib
#include <safetyhook.hpp>
#include <httplib/httplib.h>

// own
#include <sdk/get_interface.hpp>
#include <sdk/CLuaShared.hpp>
#include <sdk/CLuaInterface.hpp>

void dll_thread(HINSTANCE instance_dll) {
	DisableThreadLibraryCalls(instance_dll);

	auto lua_shared = reinterpret_cast<CLuaShared*>(get_interface("lua_shared.dll", "LUASHARED003"));

	httplib::Server server;

	server.Post("/script", [lua_shared](const httplib::Request& request, httplib::Response& response) {
		auto lua_interface = reinterpret_cast<CLuaInterface*>(lua_shared->get_current_lua_interface());

		if (const char* result = lua_interface->run_buffer(request.body.c_str())) {
			response.set_content(result, "text/plain");
		}
	});

	server.listen("127.0.0.1", PORT);

	FreeLibrary(instance_dll);
}

BOOL DllMain(HINSTANCE instance_dll, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		std::thread(dll_thread, instance_dll).detach();
	}

    return TRUE;
}