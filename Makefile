build:
	clang main.c utils.c json.c -o bin/jdif

install: build
	mkdir -p $(PREFIX)/bin
	cp bin/jdif $(PREFIX)/bin/jdif

binary: build
	sudo cp bin/jdif /usr/local/bin

example: build
	cat t/example.1.ldif | bin/jdif

test: build
	t/test.sh