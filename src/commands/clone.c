#include <stdio.h>
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
#include "utils.h"
#include "git2.h"


int _clone(const char *url, const char *path) {
	git_repository *repo = NULL;

	printf("---CLONE\n");
	if (!url || !path) {
		printf("WTF\n");
		return -1;
	}

	git_clone_options cl_opts = GIT_CLONE_OPTIONS_INIT;
	git_checkout_options co_opts = GIT_CHECKOUT_OPTIONS_INIT;
	cl_opts.checkout_opts = co_opts;
	cl_opts.fetch_opts.callbacks.credentials = credential_cb;

	int features = git_libgit2_features();
	printf("=== FEATURES ===\n");
	printf(" - https: %d\n", (features & GIT_FEATURE_HTTPS) != 0);
	printf(" - ssh: %d\n", (features & GIT_FEATURE_SSH) != 0);

	LG(git_clone(&repo, url, path, &cl_opts), "clone");
	list_branches(repo);

	return 0;
}
