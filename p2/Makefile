.PHONY: main

src/utils:
	gcc src/utils.c -o src/utils -Iincl -c

main: src/utils
	gcc src/main.c src/utils -o main -Iincl
