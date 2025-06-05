#include <lua5.2/lua.h>
#include "utils.h"
#include "anno_commit.h"
#include "git2.h"

int _stash_create(const char *message, const char *path) {
	git_repository *repo = NULL;
	git_signature *sig = NULL;
	git_oid oid;

	LG(open_repo(&repo, path), "repo");

	if (!message) {
		git_reference *ref = NULL;
		git_annotated_commit *c = NULL;

		LG(git_repository_head(&ref, repo), "head");
		LG(git_annotated_commit_from_ref(&c, repo, ref), "commit");

		git_commit *commit = c->commit;
		if (NULL == commit || NULL == (message = git_commit_summary(commit))) {
			return -3;
		}
	}

	LG(git_signature_now(&sig, "Shapillon", "sasha.fungoid@gmail.com"), "sig");

	int opts = GIT_STASH_INCLUDE_UNTRACKED;
	LG(git_stash_save(&oid, repo, sig, message, opts), "save");

	return 0;
}

int _stash_push() {
	perror("NOT IMPLEMENTED\n");
	return -1;
}

typedef enum {
	LIST,
	SHOW
} stash_cmd;

int stash_cb(size_t idx, const char *message, const git_oid *oid, void *payload) {
	if (!payload) {
		return -1;
	}

	cb_payload *p = payload;
	if (p->cmd == LIST) {
		printf("%lu %s\n", idx, message);
	} else if (p->cmd == SHOW) {
		const size_t *target_idx = p->payload;
		git_repository *repo = p->repo;

		if (!repo) {
			return -1;
		}

		if (*target_idx == idx) {
			//FIXME: handle unindexed files
			git_commit *scommit = NULL, *tcommit = NULL;
			git_tree *stree = NULL, *ttree = NULL;

			LG(git_commit_lookup(&scommit, repo, oid), "stash commit")
			LG(git_commit_tree(&stree, scommit), "stree")
			LG(git_commit_parent(&tcommit, scommit, 1), "get parent");
			LG(git_commit_tree(&ttree, tcommit), "ttree");

			git_diff_options opts;
			LG(git_diff_options_init(&opts, GIT_DIFF_OPTIONS_VERSION), "diff opts");
			opts.flags |= GIT_DIFF_INCLUDE_IGNORED;
			opts.flags |= GIT_DIFF_INCLUDE_UNTRACKED;
			opts.flags |= GIT_DIFF_SHOW_UNTRACKED_CONTENT;

			git_diff *diff = NULL;
			LG(git_diff_tree_to_tree(&diff, repo, ttree, stree, &opts), "diff");
			printf("add: %lu, rm: %lu, mv: %lu\n",
					git_diff_num_deltas_of_type(diff, GIT_DELTA_ADDED),
					git_diff_num_deltas_of_type(diff, GIT_DELTA_DELETED),
					git_diff_num_deltas_of_type(diff, GIT_DELTA_MODIFIED)
			);

			printf("ENTRY COUNTS: %lu %lu\n", git_tree_entrycount(ttree), git_tree_entrycount(stree));
			int count = git_tree_entrycount(stree);
			int i = 0;
			while (i < count) {
				printf(" - %s\n", git_tree_entry_name(git_tree_entry_byindex(stree, i)));
				++i;
			}

			return 1;
		}
	} else {
		return -1;
	}

	return 0;
}

//TODO: handle branch
int _stash_list(const char *branch, const char *path) {
	git_repository *repo = NULL;
	cb_payload *payload = NULL;

	LG(open_repo(&repo, path), "open repo");
	LG(mk_payload(&payload, LIST, repo, (void *)branch), "mk payload");
	LG(git_stash_foreach(repo, stash_cb, payload), "stash foreach");

	return 0;
}

int _stash_show(const size_t stash_no, const char *path) {
	git_repository *repo = NULL;
	cb_payload *payload = NULL;

	LG(open_repo(&repo, path), "open repo");
	LG(mk_payload(&payload, SHOW, repo, (void*)&stash_no), "mk payload");
	LG(git_stash_foreach(repo, stash_cb, payload), "stash foreach");

	return 0;
}

int _stash_drop(const size_t stash_no, const char *path) {
	git_repository *repo = NULL;

	LG(open_repo(&repo, path), "repo");
	LG(git_stash_drop(repo, stash_no), "stash drop");
	return 0;
}

int stash_apply_options(git_stash_apply_options *opts) {
	LG(git_stash_apply_options_init(opts, GIT_STASH_APPLY_OPTIONS_VERSION), "init opts");
	opts->flags = GIT_STASH_APPLY_REINSTATE_INDEX;
	return 0;
}

int _stash_apply(const size_t stash_no, const char *path) {
	git_repository *repo = NULL;
	git_stash_apply_options opts;

	LG(open_repo(&repo, path), "repo");
	LG(stash_apply_options(&opts), "init opts");
	LG(git_stash_apply(repo, stash_no, &opts), "stash apply");
	return 0;
}

int _stash_pop(const size_t stash_no, const char *path) {
	git_repository *repo = NULL;
	git_stash_apply_options opts;

	LG(open_repo(&repo, path), "repo");
	LG(stash_apply_options(&opts), "init opts");
	LG(git_stash_pop(repo, stash_no, &opts), "stash pop");
	return 0;
}
