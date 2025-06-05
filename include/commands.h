#include <lua5.2/lua.h>

#ifndef _COMMANDS_H_
# define _COMMANDS_H_

int _clone(const char *url, const char *path);

int _fetch(const char *remote_name, const char *path);
int _checkout(char *target, const char *path);

int _add(const char *fname, const char *path);
int _remove(const char *fname, const char *path);

int _status(const char *path);
int _commit(const char *message, const char *path);
int _amend(const char *message, const char *path);

int _push(const char *remote_name, const char *branch_name, const char *path);
int _pull(const char *remote_name, const char *branch_name, const char *path);

int _create_branch(const char *branch_name, const char *path);
int _move_branch(const char *dst, const char *src, const int force, const char *path);
int _delete_branch(const char *name, const char *path);
int _delete_remote_branch(const char *remote_name, const char *name, const char *path);

int _stash_create(const char *message, const char *path);
int _stash_push();
int _stash_list(const char *branch, const char *path);
int _stash_show(const size_t stash_no, const char *path);
int _stash_drop(const size_t stash_no, const char *path);
int _stash_pop(const size_t stash_no, const char *path);
int _stash_apply(const size_t stash_no, const char *path);

int _reset_soft(const char *refish, const char *path);
int _reset_hard(const char *refish, const char *path);

#endif
