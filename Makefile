build:
	clang main.c utils.c json.c -o bin/jdif

binary: build
	sudo cp bin/jdif /usr/local/bin

example: build
	cat t/example.1.ldif | bin/jdif | jq

test: build
	t/test.sh