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

SRCS += chain-hash.c
SRCS += Singly-Linked-List/list.c
OBJS = $(patsubst %.c,%.o,$(SRCS))
CFLAGS = -g -Wall -O0 -ISingly-Linked-List/ -DCONFIG_DEBUG_CHAIN_HASH
CC = gcc

.PHONY: force clean

all: force chain-hash

chain-hash: $(OBJS)

$(OBJS): force

force:

clean: force
	rm -f *.o
	rm -f chain-hash
	rm -f *.dSYM
	$(MAKE) -C Singly-Linked-List/ clean

###############################################################################
