build:
	clang main.c utils.c json.c -o bin/jdif

binary: build
	sudo cp bin/jdif /usr/local/bin

example: build
	cat example.ldif | bin/jdif | jq

test: build
	t/test.sh