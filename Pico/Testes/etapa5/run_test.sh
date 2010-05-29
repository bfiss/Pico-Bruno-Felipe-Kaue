#!/bin/bash
if test $# -ne 1
then
	echo "usage: $0 program_name"
	echo "given: $*"
else
	prog_name=$1
	./../../src/pico -o "$prog_name.tac" "$prog_name.pico"
	python "tac2x86.py" "$prog_name.tac" > /dev/null
	as output.s -o output.o
	ld -dynamic-linker /lib/ld-linux.so.2 -o "$prog_name" -lc output.o
	chmod +x "$prog_name"
fi

