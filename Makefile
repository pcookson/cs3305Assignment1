all: shell

shell: main.o token.o
	gcc main.o token.o -o shell

main.o: main.c
	gcc -c -g main.c

token.o: token.c
	gcc -c -g token.c

clean:
	rm -rf *o shell
