default: control client

control: control.c
	gcc -o control control.c

client: client.c
	gcc -o client client.c

clean:
	-rm *~
	-rm control
	-rm client
