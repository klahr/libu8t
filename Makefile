.PHONY: compile_commands clean

CC      = cc
CFLAGS  = -Iinclude -Wall -g
SRC     := $(wildcard src/*.c)
OBJDIR  := obj
OBJS    := $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))

compile_commands: $(OBJS)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	bear --append -- $(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) compile_commands.json
