CC      ?= $(CROSS_COMPILE)gcc
LD      ?= $(CROSS_COMPILE)ld
AR      ?= $(CROSS_COMPILE)ar
NM      ?= $(CROSS_COMPILE)nm
OBJCOPY ?= $(CROSS_COMPILE)objcopy
OBJDUMP ?= $(CROSS_COMPILE)objdump
READELF ?= $(CROSS_COMPILE)readelf

# OP-TEE client paths
OPTEE_CLIENT ?= $(HOME)/optee/optee_client
TEEC_INCLUDE := $(OPTEE_CLIENT)/libteec/include
TEEC_LIB     := $(OPTEE_CLIENT)/out/libteec
TEEC_EXPORT  := $(HOME)/optee/optee_client/out/export

TA_DEV_KIT_DIR := $(HOME)/optee/optee_os/out/arm/export-ta_arm64

OBJS = main.o

CFLAGS += -Wall
CFLAGS += -I../ta/include
CFLAGS += -I$(TEEC_INCLUDE)
CFLAGS += -I./include

# Link OP-TEE Client library
LDADD += -L$(TEEC_LIB)
LDADD += -lteec

BINARY = optee_example_tinyml

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $< $(LDADD)

.PHONY: clean
clean:
	rm -f $(OBJS) $(BINARY)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
