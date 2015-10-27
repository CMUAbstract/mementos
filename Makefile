#
### Mementos build system.
#
# To build with hardware support, define HARDWARE:
#  make HARDWARE=1 [...]
# To build with hardware support *and* support for logging to an outboard
# device (recommended), run:
#  make HARDWARE=1 LOGGING=1 [...]
#

include Makefile.common

MYLIBS = msp430builtins.o
MCLIBS += $(MYLIBS)

.DUMMY: all clean distclean gdbcommands install strip

MEMENTOS_SRCS =

ifndef USER_MAIN
MEMENTOS_SRCS += mementos_main.c
endif

ifdef INIT_FUNC
override CFLAGS += -DINIT_FUNC
endif

ifdef VOLTAGE_CHECK
override CFLAGS += -DMEMENTOS_VOLTAGE_CHECK
endif

ifdef TIMER
override CFLAGS += -DMEMENTOS_TIMER
endif

ifdef HARDWARE
override CFLAGS += -DMEMENTOS_HARDWARE
MEMENTOS_SRCS += mementos_hw.c
endif

ifdef LOGGING
override CFLAGS += -DMEMENTOS_LOGGING
MEMENTOS_SRCS += mementos_logging.c
endif

ifdef FRAM
override CFLAGS += -DMEMENTOS_FRAM
MEMENTOS_SRCS += mementos_fram.c
else
override CFLAGS += -DMEMENTOS_FLASH
MEMENTOS_SRCS += mementos_flash.c
endif

MEMENTOS_OBJS = $(MEMENTOS_SRCS:.c=.bc)

VTHRESHDEFAULT=2.7
ifeq ($(VTHRESH),)
 override VTHRESH=$(VTHRESHDEFAULT)
endif

TIMERINTDEFAULT=20000
ifeq ($(TIMERINT),)
 override TIMERINT=$(TIMERINTDEFAULT)
endif

# which flavors to build
FLAVORS=latch return timer+latch oracle raw
TARGETS=$(MYLIBS) $(foreach flavor,$(FLAVORS),mementos+$(flavor).bc)

all: $(TARGETS)

mementos.c: include/mementos.h

.c.bc: include/mementos.h
	$(CLANG) $(CFLAGS) -c -o $@ $<

install: $(TARGETS)
ifeq ($(MSPSIM),)
	@echo "No mspsim directory defined, so nowhere to install."
	@echo "Reconfigure --with-mspsim=/path/to/mspsim to specify" \
		"an installation path for built programs." 1>&2
else
	for t in $(TARGETS); do \
		install -m 0755 $$t $(MSPSIM)/firmware/`basename "$$t"`; \
	done
endif

# standalone Mementos bitcode (for linking against)
mementos+latch.bc: mementos.c $(MEMENTOS_OBJS)
	$(CLANG) $(CFLAGS) -o mementos.bc -DMEMENTOS_LATCH -c $<
	$(LLVM_LINK) -o $@ mementos.bc $(MEMENTOS_OBJS)
mementos+return.bc: mementos.c $(MEMENTOS_OBJS)
	$(CLANG) $(CFLAGS) -o mementos.bc -DMEMENTOS_RETURN -c $<
	$(LLVM_LINK) -o $@ mementos.bc $(MEMENTOS_OBJS)
mementos+timer+latch.bc: mementos.c $(MEMENTOS_OBJS)
	$(CLANG) $(CFLAGS) -o mementos.bc -DMEMENTOS_TIMER -DMEMENTOS_LATCH -c $<
	$(LLVM_LINK) -o $@ mementos.bc $(MEMENTOS_OBJS)
mementos+oracle.bc: mementos.c $(MEMENTOS_OBJS)
	$(CLANG) $(CFLAGS) -o mementos.bc -DMEMENTOS_ORACLE -c $<
	$(LLVM_LINK) -o $@ mementos.bc $(MEMENTOS_OBJS)
mementos+raw.bc: mementos.c $(MEMENTOS_OBJS)
	$(CLANG) $(CFLAGS) -o mementos.bc -DMEMENTOS_ORACLE -c $<
	$(LLVM_LINK) -o $@ mementos.bc $(filter-out mementos_main.bc,$(MEMENTOS_OBJS))

msp430builtins.o: msp430builtins.S
	$(GCC) $(GCC_CFLAGS) -c -o $@ $<

strip: $(TARGETS)
	msp430-strip $(TARGETS)

clean:
	$(RM) $(TARGETS) *.o *.bc *.s

distclean:
	$(RM) config.log config.status
