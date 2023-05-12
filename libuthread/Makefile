# Target library
lib := libuthread.a

# Variables
objs := queue.o uthread.o context.o sem.o preempt.o

CC := gcc
CFLAGS := -Wall -Wextra -Werror -MMD

ifneq ($(V), 1)
Q = @
endif

all: $(lib)

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

## TODO: Phase 1
%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

$(lib): $(objs)
	@echo "AR $@"
	$(Q)ar rcs $(lib) $(objs)

clean:
	@echo "CLEAN"
	$(Q)rm -f $(objs) $(deps)