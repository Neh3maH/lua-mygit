#include <lua5.2/lua.h>

#ifndef _TOAST_H_
#define _TOAST_H_

int luaopen_toast(lua_State *l);

#define LUA_EXTRALIBS { "toast", luaopen_toast }

#endif
