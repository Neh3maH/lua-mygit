#include <stdio.h>
#include <string.h>
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
#include "git2.h"
#include "utils.h"
#include "commands.h"
#include "anno_commit.h"

static int hello(lua_State *l) {
	const char *c = lua_tostring(l, 1);
	printf("hello: %s\n", c);
	return 1;
}

static int init(lua_State *l) {
	printf("git init\n");
	git_libgit2_init();
	return 0;
}

static int shutdown(lua_State *l) {
	printf("git shutdown\n");
	git_libgit2_shutdown();
	return 0;
}

static int fetch(lua_State *l) {
	const char *path = lua_tostring(l, 1);
	const char *remote_name = "origin";
	lua_settop(l, 0);

	return return_int(l, _fetch(remote_name, path));
}

static int checkout(lua_State *l) {
	const char *target = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	return return_int(l, _checkout((char *)target, path));
}

static int commit(lua_State *l) {
	const char *message = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	LUA_ASSERTP(message, "message required");
	return return_int(l, _commit(message, path));
}

static int amend(lua_State *l) {
	const char *message = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	//TODO: handle no message
	LUA_ASSERTP(message, "message required");
	return return_int(l, _amend(message, path));
}

static int push(lua_State *l) {
	const char *remote_name = arg_string(l, 1, "origin");
	const char *branch_name = lua_tostring(l, 2);
	const char *path = lua_tostring(l, 3);
	lua_settop(l, 0);

	//TODO: handle no branch
	LUA_ASSERTP(branch_name, "branch name required");
	return return_int(l, _push(remote_name, branch_name, path));
}

static int pull(lua_State *l) {
	const char *remote_name = arg_string(l, 1, "origin");
	const char *branch_name = lua_tostring(l, 2);
	const char *path = lua_tostring(l, 3);
	lua_settop(l, 0);

	LUA_ASSERTP(branch_name, "branch name required");
	return return_int(l, _pull(remote_name, branch_name, path));
}

static int clone(lua_State *l) {
	const char *url = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	LUA_ASSERTP(url, "url required");
	return return_int(l, _clone(url, path));
}

static int add(lua_State *l) {
	const char *fname = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	return return_int(l, _add(fname, path));
}

static int remove_cmd(lua_State *l) {
	const char *fname = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	return return_int(l, _remove(fname, path));
}

static int create_branch(lua_State *l) {
	const char *name = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	LUA_ASSERTP(name, "branch name required");
	return return_int(l, _create_branch(name, path));
}

static int move_branch(lua_State *l) {
	const char *dst = lua_tostring(l, 1);
	const char *src = lua_tostring(l, 2);
	const int force = 0; // lua_toboolean(l, 3);
	const char *path = lua_tostring(l, 3);
	lua_settop(l, 0);

	LUA_ASSERTP(dst, "dest name required");
	return return_int(l, _move_branch(dst, src, force, path));
}

static int delete_branch(lua_State *l) {
	const char *name = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	LUA_ASSERTP(name, "branch name required");
	return return_int(l, _delete_branch(name, path));
}

static int delete_remote_branch(lua_State *l) {
	const char *remote = arg_string(l, 1, "origin");
	const char *branch = lua_tostring(l, 2);
	const char *path = lua_tostring(l, 3);
	lua_settop(l, 0);

	LUA_ASSERTP(branch, "branch name required");
	return return_int(l, _delete_remote_branch(remote, branch, path));
}

static int list_branches_cmd(lua_State *l) {
	const char *path = lua_tostring(l, 1);
	git_repository *repo = NULL;
	lua_settop(l, 0);

	LUA_ASSERT(open_repo(&repo, path), "repo");
	return return_int(l, list_branches(repo));
}

static int status(lua_State *l) {
	const char *path = lua_tostring(l, 1);
	lua_settop(l, 0);

	return return_int(l, _status(path));
}

static int stash_create(lua_State *l) {
	const char *message = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	return return_int(l, _stash_create(message, path));
}

static int stash_push(lua_State *l) {
	return return_int(l, _stash_push());
}

static int stash_list(lua_State *l) {
	const char *branch = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	return return_int(l, _stash_list(branch, path));
}

static int stash_show(lua_State *l) {
	size_t stash_no = 0;
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	LUA_ASSERT(arg_unsigned(l, 1, &stash_no), "required stash no");
	return return_int(l, _stash_show(stash_no, path));
}

static int stash_drop(lua_State *l) {
	size_t stash_no = 0;
	const char *path = lua_tostring(l, 2);

	LUA_ASSERT(arg_unsigned(l, 1, &stash_no), "required stash no");
	return return_int(l, _stash_drop(stash_no, path));
}

static int stash_pop(lua_State *l) {
	size_t stash_no = 0;
	const char *path = lua_tostring(l, 2);

	LUA_ASSERT(arg_unsigned(l, 1, &stash_no), "required stash no");
	return return_int(l, _stash_pop(stash_no, path));
}

static int stash_apply(lua_State *l) {
	size_t stash_no = 0;
	const char *path = lua_tostring(l, 2);

	LUA_ASSERT(arg_unsigned(l, 1, &stash_no), "required stash no");
	return return_int(l, _stash_apply(stash_no, path));
}

static int reset_hard(lua_State *l) {
	const char *refish = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	LUA_ASSERTP(refish, "required refish");
	return return_int(l, _reset_hard(refish, path));
}

static int reset_soft(lua_State *l) {
	const char *refish = lua_tostring(l, 1);
	const char *path = lua_tostring(l, 2);
	lua_settop(l, 0);

	LUA_ASSERTP(refish, "required refish");
	return return_int(l, _reset_soft(refish, path));
}

static const struct luaL_Reg mylib [] = {
	{"hello_world", hello},
	{"init", init},
	{"shutdown", shutdown},
	{"clone", clone},
	{"fetch", fetch},
	{"checkout", checkout},
	{"branch_create", create_branch},
	{"branch_move", move_branch},
	{"branch_delete", delete_branch},
	{"branch_delete_remote", delete_remote_branch},
	{"branch_list", list_branches_cmd},
	{"push", push},
	{"pushd", delete_remote_branch},
	{"pull", pull},
	{"add", add},
	{"remove", remove_cmd},
	{"stash_create", stash_create},
	{"stash_push", stash_push},
	{"stash_list", stash_list},
	{"stash_show", stash_show},
	{"stash_drop", stash_drop},
	{"stash_apply", stash_apply},
	{"stash_pop", stash_pop},
	{"reset_hard", reset_hard},
	{"reset_soft", reset_soft},
	{"status", status},
	{"commit", commit},
	{"amend", amend},
	{NULL, NULL}
};

int luaopen_toast(lua_State *l) {
	luaL_newlib(l, mylib);
	lua_setglobal(l, "toastlib");
	return 1;
}
