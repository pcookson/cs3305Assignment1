all: shell

shell: shell.o token.o
	gcc shell.o token.o -o shell

shell.o: shell.c
	gcc -c -g shell.c

token.o: token.c
	gcc -c -g token.c

clean:
	rm -rf *o shell
