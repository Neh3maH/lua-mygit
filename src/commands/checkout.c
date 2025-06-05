#include <lua5.2/lua.h>
#include "git2.h"
#include "utils.h"
#include "anno_commit.h"

int _checkout(const char *remote_name, const char *branch_name, const char *path) {
	git_reference *new_branch = NULL;
	git_repository *repo = NULL;
	git_reference *ref = NULL;
	git_annotated_commit *annotated = NULL;
	char *remote = NULL;
	git_checkout_options opts;
	int error = 0;

	if (!branch_name)
		return -1;

	LG(open_repo(&repo, path), "repo");
	LG(git_checkout_options_init(&opts, GIT_CHECKOUT_OPTIONS_VERSION), "init opts");
	opts.checkout_strategy = GIT_CHECKOUT_SAFE;

	if ((error = get_commit(&annotated, repo, branch_name))) {
		if (!remote_name)
			return -1;
		LG(get_remote_branch(&remote, remote_name, branch_name), "remote_name");
		LG(get_commit(&annotated, repo, remote), "lookup remote");
	}

	git_commit *commit = annotated->commit;
	LG(git_checkout_tree(repo, (const git_object*)commit, &opts), "checkout tree");

	const char *commit_ref = git_annotated_commit_ref(annotated);
	if (commit_ref) {
		const char *target_name = NULL;

		if (git_reference_is_remote(ref)) {
			LG(git_branch_create_from_annotated(&new_branch, repo, branch_name, annotated, 0), "branch_create");
			target_name = git_reference_name(new_branch);
			git_reference_free(new_branch);
		} else
			target_name = git_annotated_commit_ref(annotated);

		LG(git_repository_set_head(repo, target_name), "set head");
	} else
		LG(git_repository_set_head_detached_from_annotated(repo, annotated), "set head detached");

	git_reference_free(ref);
	git_commit_free(commit);
	git_annotated_commit_free(annotated);
	return error;
}
