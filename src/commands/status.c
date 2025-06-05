#include "git2.h"
#include "utils.h"

int status_cb(const char *path, unsigned int status_flags, void *payload) {
	unsigned int new = status_flags & GIT_STATUS_WT_NEW;
	unsigned int modified = status_flags & GIT_STATUS_WT_MODIFIED;
	unsigned int renamed = status_flags & GIT_STATUS_WT_RENAMED;
	unsigned int deleted = status_flags & GIT_STATUS_WT_DELETED;
	unsigned int ignored = status_flags & GIT_STATUS_IGNORED;
	unsigned int conflicted = status_flags & GIT_STATUS_CONFLICTED;

	char state;
	if (new) {
		state = 'N';
	} else if (modified) {
		state = 'E';
	} else if (renamed) {
		state = 'M';
	} else if (deleted) {
		state = 'D';
	}

	const char *s_ignored = ignored ? "ignored" : "";
	const char *s_conflicted = conflicted ? "conflicted": "";

	const char *open = (ignored || conflicted) ? "[" : "";
	const char *close = (ignored || conflicted) ? "]" : "";
	const char *sep = (ignored && conflicted) ? ", " : "";
	
	printf("%c %s%s%s%s%s\t%s\n", state, open, s_ignored, sep, s_conflicted, close, path);
	return 0;
}

int _status(const char *path) {
	git_repository *repo = NULL;

	LG(open_repo(&repo, path), "repo");
	LG(git_status_foreach(repo, status_cb, NULL), "status_foreach");

	return 0;
}
