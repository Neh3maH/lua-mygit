#include <stdio.h>
#include <string.h>
#include "git2.h"
#include "utils.h"

int _pull(const char *remote_name, const char *branch_name, const char *path) {
	git_repository *repo = NULL;
	git_remote *remote = NULL;
	git_remote_callbacks cbs;
	git_fetch_options f_opts;

	LG(open_repo(&repo, path), "repo");
	LG(git_remote_lookup(&remote, repo, remote_name), "remote looup");
	LG(git_fetch_options_init(&f_opts, GIT_FETCH_OPTIONS_VERSION), "fetch options");
	LG(git_remote_init_callbacks(&cbs, GIT_REMOTE_CALLBACKS_VERSION), "callbacks init");
	cbs.credentials = credential_cb;
	f_opts.callbacks = cbs;

	const char *fmt = "refs/heads/%s:refs/heads/%s";
	const size_t len = strlen(fmt) + 2 * strlen(branch_name);
	char *ref = malloc(sizeof(char) * len);
	if (!ref)
		return -1;
	LG(snprintf(ref, len, fmt, branch_name, branch_name), "snprintf");
	const git_strarray refspecs = {&ref, 1};
	LG(git_remote_fetch(remote, &refspecs, &f_opts, NULL), "remote fetch");

	return 0;
}
