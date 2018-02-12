###############################################################################
# NAME:		    Makefile
#
# AUTHOR:	    Ethan D. Twardy
#
# DESCRIPTION:	    Makefile for the project
#
# CREATED:	    02/12/2018
#
# LAST EDITED:	    02/12/2018
###

SRCS += src/chain-hash.c
SRCS += src/linkedlist.c
OBJS = $(patsubst %.c,%.o,$(SRCS))
CFLAGS = -g -Wall -O0 -Iinclude/ -D CONFIG_DEBUG_CHAIN_HASH
CC = gcc

.PHONY: force clean

all: force src/chain-hash
	@mv -f src/chain-hash ./

src/chain-hash: $(OBJS)

$(OBJS): force

force:

clean: force
	rm -f src/*.o
	rm -f chain-hash
	rm -f *.dSYM

###############################################################################
