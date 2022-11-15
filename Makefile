CC=gcc
CFLAGS=-I src/ -pthread

app: main.o fridge.o
	$(CC) $(CFLAGS) -o app main.o fridge.o

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c

fridge.o: src/fridge.c
	$(CC) $(CFLAGS) -c src/fridge.c

unit_test: src/fridge.c tests/unit_test.c
	$(CC) $(CFLAGS) -o unit_test tests/unit_test.c src/fridge.c -pthread

clean:
	rm -rf *.o src/*.gch ./app ./unit_test