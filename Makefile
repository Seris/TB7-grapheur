build:
	clang ./*.c ./**/*.c -Iinclude -Wall -Werror -o graph

run:
	./graph
