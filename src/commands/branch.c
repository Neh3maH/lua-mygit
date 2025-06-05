#include <lua5.2/lua.h>
#include "git2.h"
#include "utils.h"

int _create_branch(const char *branch_name, const char *path) {
	git_repository *repo = NULL;
	git_annotated_commit *commit = NULL;
	git_reference *ref = NULL, *nref = NULL;

	LG(open_repo(&repo, path), "repo");
	LG(get_commit(&commit, repo, branch_name), "commit");
	LG(git_repository_head(&ref, repo), "head");
	LG(git_annotated_commit_from_ref(&commit, repo, ref), "anno commit");
	LG(git_branch_create_from_annotated(&nref, repo, branch_name, commit, 0), "create branch");

	return 0;
}

int _move_branch(const char *dst, const char *src, const int force, const char *path) {
	git_repository *repo = NULL;
	git_reference *ref = NULL, *nref = NULL;

	LG(open_repo(&repo, path), "repo");

	if ((GIT_ENOTFOUND != git_branch_lookup(&nref, repo, dst, GIT_BRANCH_LOCAL))) {
		git_reference_free(nref);
		return -2;
	}

	if (src) {
		LG(git_branch_lookup(&ref, repo, src, GIT_BRANCH_LOCAL), "branch lookup");
	} else {
		LG(git_repository_head(&ref, repo), "head");
	}

	LG(git_branch_move(&nref, ref, dst, force), "branch move");

	git_reference_free(ref);

	return 0;
}

int _delete_branch(const char *name, const char *path) {
	git_repository *repo = NULL;
	git_reference *branch = NULL;

	if (!name) {
		return -1;
	}

	LG(open_repo(&repo, path), "repo");
	LG(git_branch_lookup(&branch, repo, name, GIT_BRANCH_LOCAL), "branch lookup");
	LG(git_branch_delete(branch), "branch delete");

	git_reference_free(branch);

	return 0;
}

int _delete_remote_branch(const char *remote_name, const char *name, const char *path) {
	git_repository *repo = NULL;
	char *remote = NULL;

	LG(open_repo(&repo, path), "repo");
	LG(get_remote_branch(&remote, remote_name, name), "get_remote");
	LG(git_remote_delete(repo, remote), "delete remote");
	return 0;
}
