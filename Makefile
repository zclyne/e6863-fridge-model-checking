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

e2e_test: src/fridge.c tests/e2e_test.c
	$(CC) $(CFLAGS) -o e2e_test tests/e2e_test.c src/fridge.c -pthread

# function-level CBMC model checking

cbmc_kkv_init:
	cd src && cbmc model_check_kkv_init.c --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_kkv_init.out

cbmc_kkv_destroy:
	cd src && cbmc model_check_kkv_destroy.c --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_kkv_destroy.out

cbmc_kkv_put:
	cd src && cbmc model_check_kkv_put.c --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_kkv_put.out

clean:
	rm -rf *.o src/*.gch ./app ./unit_test