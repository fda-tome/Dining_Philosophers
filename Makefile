monitor.o: monitor.c
	gcc -c monitor.c -lpthread -o monitor.o

main.o: main.c
	gcc -c main.c -lpthread -o main.o

DPP: main.o monitor.o
	gcc -lpthread -o DPP *.o

all: DPP

run: all
	./DPP

clean:
	rm *.o DPP
