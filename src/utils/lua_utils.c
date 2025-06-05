#include <lua5.2/lua.h>
#include "git2.h"

int return_int_ext(lua_State *l, int ret, const char *loc) {
	const git_error *error = git_error_last();
	lua_pushinteger(l, ret);

	if (loc)
		lua_pushstring(l, loc);

	if (error && error->klass != 0) {
		lua_pushinteger(l, error->klass);
		lua_pushstring(l, error->message);
	}

	return lua_gettop(l);
}

int return_int(lua_State *l, int ret) {
	return return_int_ext(l, ret, NULL);
}

const char *arg_string(lua_State *l, int idx, const char *orelse) {
	const char *ret = lua_tostring(l, idx);
	return ret ? ret : orelse;
}

const int arg_int(lua_State *l, int idx, int *dest) {
	int error = 0;
	*dest = lua_tointegerx(l, idx, &error);
	return error ? 0 : -1;
}

const int arg_unsigned(lua_State *l, int idx, size_t *dest) {
	int error = 0;
	*dest = lua_tounsignedx(l, idx, &error);
	return error ? 0: -1;
}
