#pragma once

#include <windows.h>
#include <string>
#include <iostream>

#include "lua/lua.hpp"

struct CLuaInterface {
	inline lua_State* lua_state() {
		return *reinterpret_cast<lua_State**>(this + 1);
	}

	virtual int get_top(void) = 0;
	virtual void push_value(int index) = 0;
	virtual void pop(int amount = 1) = 0;
	virtual void get_table(int index) = 0;
	virtual void get_field(int index, const char* name) = 0;
	virtual void set_field(int index, const char* name) = 0;
	virtual void create_table() = 0;
	virtual void set_table(int index) = 0;
	virtual void set_metatable(int index) = 0;
	virtual bool get_metatable(int index) = 0;
	virtual void call(int argument_amount, int result_amount) = 0;
	virtual int pcall(int argument_amount, int result_amount, int error_function) = 0;
	virtual int equal(int a_index, int b_index) = 0;
	virtual int raw_equal(int a_index, int b_index) = 0;
	virtual void insert(int index) = 0;
	virtual void remove(int index) = 0;
	virtual int next(int index) = 0;
	virtual void* new_userdata(unsigned int size) = 0;
	virtual void throw_error(const char* error_string) = 0;
	virtual void check_type(int index, int type) = 0;
	virtual void argument_error(int argument_position, const char* message_string) = 0;
	virtual void raw_get(int index) = 0;
	virtual void raw_set(int index) = 0;

	virtual const char* get_string(int index = -1, unsigned int* length = NULL) = 0;
	virtual double get_number(int index = -1) = 0;
	virtual bool get_boolean(int index = -1) = 0;
	virtual lua_CFunction get_c_function(int index = -1) = 0;
	virtual void* get_userdata(int index = -1) = 0;

	virtual void push_nil() = 0;
	virtual void push_string(const char* value, unsigned int length = 0) = 0;
	virtual void push_number(double value) = 0;
	virtual void push_boolean(bool value) = 0;
	virtual void push_c_function(lua_CFunction value) = 0;
	virtual void push_c_closure(lua_CFunction value, int variables) = 0;
	virtual void push_userdata(void*) = 0;

	virtual int reference_create() = 0;
	virtual void reference_free(int reference_index) = 0;
	virtual void reference_push(int reference_index) = 0;

	virtual void push_special(int type) = 0;

	virtual bool is_type(int index, int type) = 0;
	virtual int get_type(int index) = 0;
	virtual const char* get_type_name(int type) = 0;

	virtual void create_metatable_type(const char* name, int type) = 0;

	virtual const char* check_string(int index = -1) = 0;
	virtual double check_number(int index = -1) = 0;

	int load_buffer(const char* buffer) {
		const auto lua_shared = GetModuleHandleA("lua_shared.dll");

		if (lua_shared == 0)
			return 0;

		auto process_address = GetProcAddress(lua_shared, "luaL_loadbuffer");
		
		if (process_address == 0)
			return 0;

		auto random_character = []() -> char { // this is NOT mine :money_mouth:
			const char character_set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
			return character_set[rand() % (sizeof(character_set) - 1)];
		};

		std::string random_name(8, 0);
		std::generate_n(random_name.begin(), 8, random_character); // this is NOT mine :money_mouth:

		using luaL_loadbuffer_t = int (*)(lua_State*, const char*, size_t, const char*);
		auto luaL_loadbuffer = reinterpret_cast<luaL_loadbuffer_t>(process_address);

		return luaL_loadbuffer(lua_state(), buffer, strlen(buffer), random_name.c_str());
	}

	const char* run_buffer(const char* buffer) {
		int result = load_buffer(buffer);
		
		if (result != LUA_OK) {
			const char* error_string = get_string();
			pop();
		
			return error_string;
		}
		
		result = pcall(0, (-1), 0);

		if (result != LUA_OK) {
			const char* error_string = get_string();
			pop();

			return error_string;
		}

		return 0;
	}
};
