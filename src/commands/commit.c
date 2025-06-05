#include "git2.h"
#include "utils.h"
#include "anno_commit.h"

typedef int (*commit_fptr)(
		git_oid* out,
		git_repository *repo,
		const git_commit *target,
		const char *ref,
		const git_signature *sig,
		const char *encoding,
		const char *msg,
		const git_tree *tree
);

int commit_cmd(const char *message, const char *path, commit_fptr fptr) {
	git_repository *repo = NULL;
	git_reference *ref = NULL;
	git_annotated_commit *annotated_commit = NULL;
	git_signature *sig = NULL;
	git_index *idx = NULL;
	git_tree *tree = NULL;
	git_oid tree_oid;
	git_oid id;

	LG(open_repo(&repo, path), "repo");
	LG(git_repository_head(&ref, repo), "head");
	LG(git_annotated_commit_from_ref(&annotated_commit, repo, ref), "annotated");
	git_commit *commit = annotated_commit->commit;
	LG(git_signature_now(&sig, "Shapillon", "sasha.fungoid@gmail.com"), "sig");
	LG(git_repository_index(&idx, repo), "index open");
	LG(git_index_write_tree(&tree_oid, idx), "index write");
	LG(git_tree_lookup(&tree, repo, &tree_oid), "tree");
	
	LG(fptr(&id, repo, commit, "HEAD", sig, "ascii", message, tree), "fptr");
	return 0;
}

int commit_create_fptr(
		git_oid* out,
		git_repository *repo,
		const git_commit *target,
		const char *ref,
		const git_signature *sig,
		const char *encoding,
		const char *msg,
		const git_tree *tree
) {
	git_commitarray parents;
	LG(git_repository_commit_parents(&parents, repo), "parents");
	LG(git_commit_create(
				out,
				repo,
				ref,
				sig,
				sig,
				encoding,
				msg,
				tree,
				parents.count,
				(const git_commit **)parents.commits
	), "commit_create");
	return 0;
}

int _commit(const char *message, const char *path) {
	LG(commit_cmd(message, path, commit_create_fptr), "commit_cmd");
	return 0;
}

int commit_amend_fptr(
		git_oid* out,
		git_repository *repo,
		const git_commit *target,
		const char *ref,
		const git_signature *sig,
		const char *encoding,
		const char *msg,
		const git_tree *tree
) {
	LG(git_commit_amend(out, target, "HEAD", sig, sig, "ascii", msg, tree), "amend");
	return 0;
}

int _amend(const char *message, const char *path) {
	LG(commit_cmd(message, path, commit_amend_fptr), "commit_cmd");
	return 0;
}
