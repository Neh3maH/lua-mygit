#include <stdio.h>
#include <lua5.2/lua.h>
#include "git2.h"

#ifndef _UTILS_H_
# define _UTILS_H_

int get_remote_branch(char **out, const char *remote, const char *branch);
int get_origin_branch(char **out, const char *branch);
char *get_local_branch(char *branch);

int open_repo(git_repository **repo, const char *path);
int get_commit(git_annotated_commit **commit, git_repository *repo, const char *refish);
int list_branches(git_repository *repo);

int return_int_ext(lua_State *l, int ret, const char *loc);
int return_int(lua_State *l, int ret);
const char *arg_string(lua_State *l, int idx, const char *orelse);
const int arg_integer(lua_State *l, int idx, int *dest);
const int arg_unsigned(lua_State *l, int idx, size_t *dest);

int credential_cb(
	git_credential **out,
	const char *url,
	const char *uname,
	unsigned int allowed_types,
	void *payload
);
int match_cb(const char *path, const char *spec, void *payload);

typedef struct {
	size_t cmd;
	void *payload;
	git_repository *repo;
} cb_payload;
int mk_payload(cb_payload **ret, size_t cmd, git_repository *repo, void *payload);

#define LG(code, msg) {const int __c = code; if (lg_call(0 > __c, __c, msg, __func__)) { return __c; }};
#define LGP(val, msg) {const void *__v = val; if (lg_call(NULL == __v, NULL == __v, msg, __func__)) { return -1; }};
#define LUA_ASSERT(val, msg) {const int __v = val; if (lg_call(!__v, __v, msg, __func__)) { return return_int(l, __v); }};
#define LUA_ASSERTP(val, msg) {const void *__v = val; if (lg_call(__v == NULL, __v == NULL, msg, __func__)) { return return_int(l, __v == NULL); }};

int lg_call(int check, int code, const char *msg, const char *loc);

#endif
