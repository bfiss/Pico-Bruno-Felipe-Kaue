#!/bin/bash
cd "../Testes/etapa5"
for prog in *.pico
do
	echo "$prog"
	prog_name=${prog%\.*}
	./run_test.sh "$prog_name"
done

