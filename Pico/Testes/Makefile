CFLAGS = -P -include "../src/tokens.h" -E

all: test_etapa1.in test_etapa1.out

test_etapa1.in: test_etapa1.c ../src/tokens.h
	gcc -o test_etapa1_temp.in $< $(CFLAGS) -D "t(x, y) = x" -D "t2(x, y) = x" -D "t3(x, y, z) = x"
	sed -e "s/\\\//g" -e "s/'\(.\)'/\1/g" < test_etapa1_temp.in > test_etapa1.in
	
test_etapa1.out: test_etapa1.c ../src/tokens.h
	gcc -o test_etapa1_temp.out $< $(CFLAGS) -D "t(x, y) = y" -D "t2(x, y) = x y" -D "t3(x, y, z) = y z"
	sed -e "s/'\(.\)'/\1/g" < test_etapa1_temp.out > test_etapa1.out

