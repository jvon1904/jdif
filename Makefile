build:
	clang main.c -o bin/jdif

binary: build
	sudo cp bin/jdif /usr/local/bin

example: build
	cat example.ldif | bin/jdif | jq

test: build
	t/test.sh