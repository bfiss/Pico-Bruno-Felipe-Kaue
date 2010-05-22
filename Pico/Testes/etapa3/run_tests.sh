#!/bin/bash
for i in {0..16}
do
	./pico "../Testes/etapa3/in$i.pico" > "../Testes/etapa3/out$i.pico" 2>&1
	diff "../Testes/etapa3/out$i.pico" "../Testes/etapa3/Out$i.pico"
	echo "Test $i OK"
done

