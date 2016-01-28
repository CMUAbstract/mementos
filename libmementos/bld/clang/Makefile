include $(MEMENTOS_ROOT)/Makefile.config

SRC_ROOT = ../../src

EXEC = libmementos.a.bc

OBJECTS = \
	mementos.o \

ifeq ($(MEMENTOS_IGNORE_GLOBALS),1)
OBJECTS += mementos_gsize.o
endif

ifeq ($(MEMENTOS_USER_MAIN),0)
OBJECTS += mementos_main.o
endif

ifeq ($(MEMENTOS_HARDWARE),1)
CFLAGS += -DMEMENTOS_HARDWARE
OBJECTS += mementos_hw.o
endif

ifeq ($(MEMENTOS_LOGGING),1)
CFLAGS += -DMEMENTOS_LOGGING
OBJECTS += mementos_logging.o
endif

ifeq ($(MEMENTOS_NVMEM_TYPE),fram)
CFLAGS += -DMEMENTOS_FRAM
OBJECTS += mementos_fram.o
else ifeq ($(MEMENTOS_NVMEM_TYPE),flash)
CFLAGS += -DMEMENTOS_FLASH
OBJECTS += mementos_flash.o
else
$(error Unrecognized MEMENTOS_NVMEM_TYPE: "$(MEMENTOS_NVMEM_TYPE)")
endif

CFLAGS += \
	-I$(SRC_ROOT)/include \
	-I$(SRC_ROOT)/include/libmementos \

ifeq ($(MEMENTOS_MODE),latch)
CFLAGS += -DMEMENTOS_LATCH
else ifeq ($(MEMENTOS_MODE),return)
CFLAGS += -DMEMENTOS_RETURN
else ifeq ($(MEMENTOS_MODE),oracle)
CFLAGS += -DMEMENTOS_ORACLE
else ifeq ($(MEMENTOS_MODE),manual)
CFLAGS += -DMEMENTOS_ORACLE
else
$(error Unrecognized MEMENTOS_MODE: "$(MEMENTOS_MODE)")
endif

ifeq ($(MEMENTOS_INIT_FUNC),1)
CFLAGS += -DMEMENTOS_INIT_FUNC
endif

ifeq ($(MEMENTOS_VOLTAGE_CHECK),1)
CFLAGS += -DMEMENTOS_VOLTAGE_CHECK
endif

ifeq ($(MEMENTOS_TIMER),1)
CFLAGS += -DMEMENTOS_TIMER
endif

include $(MAKER_ROOT)/Makefile.clang
