# To build against a <program>.c (default: samples/crc-vanilla.c), run:
#  make -f Makefile.app TARGET=program

include Makefile.common

# conditional definitions
TARGET ?= samples/crc-vanilla

FLAVORS=plainclang plainmspgcc latch return timer oracle
TARGETS=$(foreach flavor,$(FLAVORS),$(TARGET)+$(flavor))

all: $(TARGETS)

# plain target built with clang
$(TARGET)+plainclang: $(TARGET).c include/mementos.h $(MYLIBS)
	$(CLANG) $(CFLAGS)   -o $@.bc -c $<
	$(LLC)            -o $@.s $@.bc
	$(GCC) $(GCC_CFLAGS) -o $@ $@.s $(MCLIBS)

# plain target built with mspgcc
$(TARGET)+plainmspgcc: $(TARGET).c include/mementos.h
	$(GCC) $(GCC_CFLAGS) -o $@ $< $(MCLIBS)

# instrument all loop latches
$(TARGET)+latch: $(TARGET).c include/mementos.h mementos+latch.bc $(MYLIBS)
	$(CLANG) $(CFLAGS)   -o $@.bc -DMEMENTOS_LATCH -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM_LINK)      -o $@+gsize+mementos.bc $@+gsize.bc mementos+latch.bc
	$(OPT_LATCH)      -o $@+gsize+mementos+o.bc $@+gsize+mementos.bc
	$(LLC)            -o $@.s $@+gsize+mementos+o.bc
	$(GCC) $(GCC_CFLAGS) -o $@ $@.s $(MCLIBS)

# instrument all function returns
$(TARGET)+return: $(TARGET).c include/mementos.h mementos+return.bc $(MYLIBS)
	$(CLANG) $(CFLAGS)   -o $@.bc -DMEMENTOS_RETURN -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM_LINK)      -o $@+gsize+mementos.bc $@+gsize.bc mementos+return.bc
	$(OPT_RETURN)     -o $@+gsize+mementos+o.bc $@+gsize+mementos.bc
	$(LLC)            -o $@.s $@+gsize+mementos+o.bc
	$(GCC) $(GCC_CFLAGS) -o $@ $@.s $(MCLIBS)

# instrument loop latches with trigger points, but these trigger points won't
# run at all (i.e., return without doing an energy check) unless the
# 'ok_to_checkpoint' flag is set, which happens every TIMER_INTERVAL
# (include/mementos.h) cycles.
$(TARGET)+timer: $(TARGET).c include/mementos.h mementos+timer+latch.bc $(MYLIBS)
	$(CLANG) $(CFLAGS)   -o $@.bc -DMEMENTOS_TIMER -DMEMENTOS_LATCH -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM_LINK)      -o $@+gsize+mementos.bc $@+gsize.bc mementos+timer+latch.bc
	$(OPT_LATCH)      -o $@+gsize+mementos+o.bc $@+gsize+mementos.bc
	$(LLC)            -o $@.s $@+gsize+mementos+o.bc
	$(GCC) $(GCC_CFLAGS) -o $@ $@.s $(MCLIBS)

# link against mementos but don't instrument code
$(TARGET)+oracle: $(TARGET).c include/mementos.h mementos+oracle.bc $(MYLIBS)
	$(CLANG) $(CFLAGS)   -o $@.bc -DMEMENTOS_ORACLE -c $<
	$(OPT_GSIZE)      -o $@+gsize.bc $@.bc
	$(LLVM_LINK)      -o $@+gsize+mementos.bc $@+gsize.bc mementos+oracle.bc
	$(LLC)            -o $@.s $@+gsize+mementos.bc
	$(GCC) $(GCC_CFLAGS) -o $@ $@.s $(MCLIBS)

logme: logme.c
	msp430-gcc -mmcu=msp430x2132 -o $@ $<

strip: $(TARGETS)
	msp430-strip $(TARGETS)

gdbcommands: $(TARGETS)
	for t in $(TARGETS); do \
		MEMENTOS_INTERESTING='restore|log_event' \
		scripts/gen-gdb-commands.sh $$t > $$t.gdb ; \
	done

clean:
	$(RM) $(TARGETS) samples/*.o samples/*.bc samples/*.s logme samples/*.gdb
