#!/bin/bash
for prog in *.pico
do
	echo "$prog"
	prog_name=${prog%\.*}
	rm $prog_name
done

