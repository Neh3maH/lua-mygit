#include "git2.h"

#ifndef _ANNO_COMMIT_H_
# define _ANNO_COMMIT_H_

typedef enum {
	GIT_ANNOTATED_COMMIT_REAL = 1,
	GIT_ANNOTATED_COMMIT_VIRTUAL = 2
} git_annotated_commit_t;

struct git_annotated_commit {
	git_annotated_commit_t type;

	/* real commit */
	git_commit *commit;
	git_tree *tree;

	/* virtual commit structure */
	git_index *index;
	git_strarray parents;

	/* how this commit was looked up */
	const char *description;

	const char *ref_name;
	const char *remote_url;

	char id_str[GIT_OID_MAX_HEXSIZE + 1];
};


#endif
