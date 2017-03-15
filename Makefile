build:
	clang -g ./*.c ./**/*.c -Iinclude -Wall -Werror -o graph -Wno-error-macro-redefined -lglut -lGL -lm

run:
	./graph
