CC      = gcc
LDFLAGS = -g
CFLAGS = -Wall $(TEST)
OBJS = lista.o node.o symbol_table.o avl.o entry.o
## descomentar a linha abaixo e chamar "make -B" e "make test" para testar
#TEST = -D "__BFK_DO_TESTING__"

all: etapa4

test: test_etapa4

## Etapa 4
etapa4: pico
	#ctags *

test_etapa4: pico

## Etapa 3
etapa3: pico

test_etapa3: pico
	../Testes/etapa3/run_tests.sh

## Etapa 2
etapa2: pico

test_etapa2: test_lista test_node test_avl test_symbol_table

test_%: ../Testes/test_%.c $(OBJS)
	gcc -o ../Testes/$@ $^ $(CFLAGS)
	./../Testes/$@

## Etapa 1
etapa1: pico

test_etapa1: pico ../Testes/test_etapa1.c
	cd ../Testes && $(MAKE)
	./$< ../Testes/test_etapa1.in > ../Testes/test_etapa1_pico_temp.out
	sed -e "s/^SPACE//g" < ../Testes/test_etapa1_pico_temp.out > ../Testes/test_etapa1_pico.out
	diff -w -b -B ../Testes/test_etapa1.out ../Testes/test_etapa1_pico.out

doc: Doxyfile $(HEAD1)
	doxygen Doxyfile
	cd ../doc/latex && $(MAKE)
	mv ../doc/latex/refman.pdf ../doc/latex/pico.pdf
	cd ../doc/latex && $(MAKE) clean

pico: lex.yy.c y.tab.c $(OBJS)
	gcc -o $@ $^ -lfl $(CFLAGS)

lex.yy.c: scanner.l tokens.h
	flex $<

y.tab.c: pico.y
	yacc -d $<
	mv y.tab.h tokens.h
	
clean:
	rm -f *.o lex.yy.? y.tab.? *~
	rm -f ../Testes/*~ ../Testes/etapa3/*~ ../Testes/etapa3/out*
	
debug: lex.yy.c y.tab.c symbol_table.c node.c
	gcc -D DEBUG -o pico y.tab.c lex.yy.c symbol_table.c node.c -ll
	
	
