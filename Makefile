IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR)
#CFLAGS+= -Wall -Wextra
CFLAGS+= -Wno-discarded-qualifiers

SDIR=src

ODIR=obj
LDIR =./libs
LIBS=-L$(LDIR) -l:libgit2.so.1.9 -l:liblua5.2.so
TARGET=toast.so

_DEPS = git2.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

CMDIR = commands
_CMDS = branch.o checkout.o clone.o\
	   commit.o fetch.o index.o\
	   push.o pull.o status.o stash.o\
	   reset.o
_OBJ = $(patsubst %, $(CMDIR)/%, $(_CMDS))
_OBJ += utils/git_utils.o utils/lua_utils.o\
		lib.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	mkdir -p $(ODIR)/utils $(ODIR)/commands
	$(CC) -shared -o $@ $^ $(CFLAGS) $(LIBS) -DLUA_USERCONFIG=\"$(IDIR)/toast.h\"

clean:
	rm -rf $(TARGET)

fclean: clean
	rm -rf $(ODIR)
	mkdir -p $(ODIR)/utils $(ODIR)/commands


re: fclean $(TARGET)

.PHONY: clean fclean re
