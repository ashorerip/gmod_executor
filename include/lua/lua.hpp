#pragma once

enum LUA_INTERFACE {
	CLIENT,
	_SERVER, // shouldn't be used
	MENU,
};

struct lua_Debug
{
	int event;
	const char* name;
	const char* name_what;
	const char* what;
	int current_line;
	int upvalue_amount;
	int line_of_definition;
	int last_line_of_definition;
	char short_src[60];
};

using lua_State = void*; // cba
using lua_Debug = struct lua_Debug;

using lua_Alloc = void* (*)(void*, void*, size_t, size_t);
using lua_CFunction = int (*)(lua_State*, int);
using lua_Hook = void (*)(lua_State*, lua_Debug*);
using lua_Integer = long long;
using lua_Number = double;
using lua_Reader = const char* (*)(lua_State*, void*, size_t*);
using lua_Writer = int (*)(lua_State*, const void*, size_t, void*);

constexpr int LUA_OK = 0;
constexpr int LUA_YIELD = 1;
constexpr int LUA_ERRRUN = 2;
constexpr int LUA_ERRSYNTAX = 3;
constexpr int LUA_ERRMEM = 4;
constexpr int LUA_ERRERR = 5;

constexpr int LUA_TNIL = 0;
constexpr int LUA_TBOOLEAN = 1;
constexpr int LUA_TLIGHTUSERDATA = 2;
constexpr int LUA_TNUMBER = 3;
constexpr int LUA_TSTRING = 4;
constexpr int LUA_TTABLE = 5;
constexpr int LUA_TFUNCTION = 6;
constexpr int LUA_TUSERDATA = 7;
constexpr int LUA_TTHREAD = 8;