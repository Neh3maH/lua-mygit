#include "git2.h"
#include "utils.h"
#include "anno_commit.h"

int reset_cmd(const char *refish, git_reset_t reset_type, int checkout_strategy, const char *path) {
	git_repository *repo = NULL;
	git_annotated_commit *anno = NULL;
	git_object *obj = NULL;
	const git_oid *oid = NULL;
	git_checkout_options opts;

	LG(open_repo(&repo, path), "repo");
	LG(get_commit(&anno, repo, refish), "get commit");
	LGP(oid = git_annotated_commit_id(anno), "commit oid");
	LG(git_object_lookup(&obj, repo, oid, GIT_OBJECT_ANY), "object_lookup");
	LG(git_checkout_options_init(&opts, GIT_CHECKOUT_OPTIONS_VERSION), "opts");
	LG(git_reset(repo, obj, reset_type, &opts), "reset");

	return 0;
}

int _reset_soft(const char *refish, const char *path) {
	int checkout_strat = GIT_CHECKOUT_SAFE;
	return reset_cmd(refish, GIT_RESET_SOFT, checkout_strat, path);
}

int _reset_hard(const char *refish, const char *path) {
	int checkout_strat = GIT_CHECKOUT_SAFE | GIT_CHECKOUT_REMOVE_UNTRACKED;
	return reset_cmd(refish, GIT_RESET_HARD, checkout_strat, path);
}
