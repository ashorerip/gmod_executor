#pragma once

#include <sdk/lua_interface.hpp>

struct CLuaShared {
	virtual void destructor() = 0;
	virtual void initialise(void* (*)(const char*, int*), bool, void*, void*) = 0;
	virtual void shut_down(void) = 0;
	virtual void dump_statistics() = 0;
	virtual void* create_lua_interface(unsigned char, bool) = 0;
	virtual void close_lua_interface(void*) = 0;
	virtual void* get_lua_interface(unsigned char) = 0;
	virtual void* load_file(const char*, const char*, bool, bool) = 0;
	virtual void* get_cache(const char*) = 0;
	virtual void mount_lua(const char*) = 0;
	virtual void mount_lua_add(const char*, const char*) = 0;
	virtual void unmount_lua(const char*) = 0;
	virtual void set_file_contents(const char*, const char*) = 0;
	virtual void set_lua_find_hook(void*) = 0;
	virtual void find_scripts(const char*, const char*, char**) = 0;
	virtual const char* get_stack_traces() = 0;
	virtual void invalidate_cache(const char*) = 0;
	virtual void empty_cache() = 0;

	CLuaInterface* get_current_lua_interface() {
		auto client_interface = reinterpret_cast<CLuaInterface*>(get_lua_interface(0));
		auto menu_interface = reinterpret_cast<CLuaInterface*>(get_lua_interface(2));

		return client_interface ? client_interface : menu_interface;
	}
};