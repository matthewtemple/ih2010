#!/bin/sh

IH=$IH

gcc stravel.bin.c -o stravel.bin -ansi -Wall -Werror -O3 -I$IH_POSTGRESQL_INCLUDE_DIR -I$IH/.. -D_GNU_SOURCE -L$IH -lih -lncurses -lm
