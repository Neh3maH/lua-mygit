#include <stdio.h>
#include <string.h>
#include "git2.h"
#include "utils.h"

int get_remote_branch(char **out, const char *remote, const char *branch) {
	const char *fmt = "%s/%s";
	const size_t len = strlen(remote) + strlen(branch) + strlen(fmt);

	*out = malloc(sizeof(char) * len);
	if (!*out)
		return -1;

	LG(snprintf(*out, len, fmt, remote, branch), "snprintf");
	return 0;
}

int get_origin_branch(char **out, const char *branch) {
	return get_remote_branch(out, "origin", branch);
}

char *get_local_branch(char *branch) {
	char *ret = strrchr(branch, '/');
	if (!ret) {
		return branch;
	}
	return ret;
}

int open_repo(git_repository **repo, const char *path) {
	if (NULL == path) {
		path = "./";
	}

	return git_repository_open(repo, path);
}

int get_commit(git_annotated_commit **commit, git_repository *repo, const char *refish) {
	git_object *obj = NULL;
	git_reference *ref = NULL;
	int err = 0;

	err = git_reference_dwim(&ref, repo, refish);
	if (err == GIT_OK) {
		err = git_annotated_commit_from_ref(commit, repo, ref);
		git_reference_free(ref);
		return err;
	}

	err = git_revparse_single(&obj, repo, refish);
	if (err == GIT_OK) {
		err = git_annotated_commit_lookup(commit, repo, git_object_id(obj));
		git_object_free(obj);
		return err;
	}

	return -1;
}

int list_branches(git_repository *repo) {
	git_branch_iterator *iter = NULL;
	git_reference *ref = NULL;
	git_branch_t t;
	const char *name;
	int count = 0;

	int error = git_branch_iterator_new(&iter, repo, GIT_BRANCH_ALL);

	if (error) {
		printf("=== list_branches: ERROR %d ===\n", error);
		count = -1;
	} else {
		printf("=== list_branches: SUCCESS===\n");
		while (!git_branch_next(&ref, &t, iter)) {
			++count;
			git_branch_name(&name, ref);
			printf(" - [%c] %s\n", (1 == t) ? 'L' : 'R' ,name);
			git_reference_free(ref);
		}
		printf("=== end ===\n");
		git_branch_iterator_free(iter);
	}

	return count;
}

int credential_cb(
		git_credential **out,
		const char *url,
		const char *uname,
		unsigned int allowed_types,
		void *payload) {
	if (allowed_types & GIT_CREDENTIAL_SSH_KEY) {
		const char *uname = "git";
		const char *privkey = "./tots";
		const char *pubkey = "./tots.pub";

		int err = git_credential_ssh_key_new(out, uname, pubkey, privkey, "");
		printf("CREDENTIALS: SSH %d\n", err);

		return err;
	} else if (allowed_types & GIT_CREDENTIAL_USERPASS_PLAINTEXT) {
		printf("CREDENTIALS: PLAINTEXT\n");
		return 1;
	}
	return 1;
}

int match_cb(const char *path, const char *spec, void *payload) {
	printf("=== match ===\n");
	printf(" - path = %s\n", path);
	printf(" - spec = %s\n", spec);
	printf("======\n");
	return 0;
}

int mk_payload(cb_payload **ret, size_t cmd, git_repository *repo, void *payload) {
	if ((!*ret && !(*ret = malloc(sizeof(cb_payload)))) || !*ret)
		return -1;

	(*ret)->cmd = cmd;
	(*ret)->payload = payload;
	(*ret)->repo = repo;
	return 0;
}

int lg_call(int check, int code, const char *msg, const char *loc) {
	if (check) {
		const git_error *error = git_error_last();
		if (error) {
			const char * fmt = "error %s in %s(%d): %d %s\n";
			fprintf(stderr, fmt, msg, loc, code, error->klass, error->message);
		}
	}
	return check;
}
