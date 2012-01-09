
all: HW1 HW2

HW1:
	gcc -g -o integer integer.c

HW2:
	gcc -g -o avltree avltree.c

clean:
	rm integer avltree
