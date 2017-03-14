build:
	clang -g ./*.c ./**/*.c -Iinclude -Wall -Werror -o graph

run:
	./graph
