#include <stdio.h>
#include "git2.h"
#include "utils.h"

typedef int (bypath_fptr)(git_index *, const char *);

int index_cmd(const char *fname, const char *path, bypath_fptr *fptr) {
	git_repository *repo = NULL;
	git_index *idx = NULL;

	LG(open_repo(&repo, path), "repo");
	LG(git_repository_index(&idx, repo), "index");

	if (fname) {
		LG(git_index_add_bypath(idx, fname), "index bypath");
	} else {
		char *paths[] = {"*"};
		git_strarray pathspec = {paths, 1};

		LG(git_index_add_all(idx, &pathspec, GIT_INDEX_ADD_DEFAULT, match_cb, NULL), "index add all");
	}

	LG(git_index_write(idx), "index write");

	return 0;
}

int _add(const char *fname, const char *path) {
	LG(index_cmd(fname, path, git_index_add_bypath), "index_cmd");
	return 0;
}

int _remove(const char *fname, const char *path) {
	if (!fname)
		return -1;
	LG(index_cmd(fname, path, git_index_remove_bypath), "index_cmd");
	return 0;
}
