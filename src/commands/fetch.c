#include <lua5.2/lua.h>
#include "git2.h"
#include "utils.h"

int _fetch(const char *remote_name, const char *path) {
	git_repository *repo = NULL;
	git_remote *remote = NULL;
	git_remote_callbacks cbs;
	git_fetch_options f_opts;

	char *ref = "refs/heads/*:refs/heads/*";
	git_strarray refspec = { &ref, 1 };

	LG(git_remote_init_callbacks(&cbs, GIT_REMOTE_CALLBACKS_VERSION), "cbs");
	LG(git_fetch_options_init(&f_opts, GIT_FETCH_OPTIONS_VERSION), "opts");
	cbs.credentials = credential_cb;
	f_opts.callbacks = cbs;

	LG(open_repo(&repo, path), "repo");
	LG(git_remote_lookup(&remote, repo, remote_name), "remote lookup");
	LG(git_remote_fetch(remote, &refspec, &f_opts, NULL), "remote fetch");

	return 0;
}
