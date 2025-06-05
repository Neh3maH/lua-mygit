#include <stdio.h>
#include <string.h>
#include "git2.h"
#include "utils.h"

int _push(const char *remote_name, const char *branch_name, const char *path) {
	git_repository *repo = NULL;
	git_remote *remote = NULL;
	git_reference *out = NULL;
	git_push_options p_opts;
	git_remote_callbacks cbs;
	git_fetch_options f_opts;


	LG(open_repo(&repo, path), "repo");
	LG(git_remote_lookup(&remote, repo, remote_name), "remote_lookup");
	LG(git_push_options_init(&p_opts, GIT_PUSH_OPTIONS_VERSION), "push opts");
	LG(git_remote_init_callbacks(&cbs, GIT_REMOTE_CALLBACKS_VERSION), "callbacks opts");
	cbs.credentials = credential_cb;
	p_opts.callbacks = cbs;
	p_opts.follow_redirects = GIT_REMOTE_REDIRECT_ALL;

	LG(git_branch_lookup(&out, repo, branch_name, GIT_BRANCH_LOCAL), "branch lookup");

	const char *fmt = "refs/heads/%s:refs/heads/%s";
	const size_t len = strlen(fmt) + 2 * strlen(branch_name);
	char *ref = malloc(sizeof(char) * len);
	if (!ref)
		return -1;
	LG(snprintf(ref, len, fmt, branch_name, branch_name), "snprintf");

	const git_strarray refspecs = { (char **)&ref, 1 };
	LG(git_remote_upload(remote, &refspecs, &p_opts), "");

	LG(git_fetch_options_init(&f_opts, GIT_FETCH_OPTIONS_VERSION), "fetch opts");
	f_opts.callbacks = cbs;
	LG(git_remote_fetch(remote, &refspecs, &f_opts, NULL), "remote_fetch");

	return 0;
}
