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
	cd src && cbmc model_check_kkv_init.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_kkv_init.out

cbmc_kkv_destroy:
	cd src && cbmc model_check_kkv_destroy.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_kkv_destroy.out

cbmc_kkv_put:
	cd src && cbmc model_check_kkv_put.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_kkv_put.out

cbmc_kkv_get:
	cd src && cbmc model_check_kkv_get.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_kkv_get.out

cbmc_program_1:
	cd src && cbmc model_check_program_1.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_program_1.out

cbmc_program_2:
	cd src && cbmc model_check_program_2.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_program_2.out

cbmc_program_3:
	cd src && cbmc model_check_program_3.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_program_3.out

cbmc_program_4:
	cd src && cbmc model_check_program_4.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_program_4.out

cbmc_program_5:
	cd src && cbmc model_check_program_5.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_program_5.out

cbmc_program_6:
	cd src && cbmc model_check_program_6.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_program_6.out

cbmc_program_7:
	cd src && cbmc model_check_program_7.c --unwind 30 --trace --bounds-check --pointer-check --memory-leak-check --div-by-zero-check --signed-overflow-check --unsigned-overflow-check --pointer-overflow-check --conversion-check  > ../cbmc_program_7.out

clean:
	rm -rf *.o src/*.gch ./app ./unit_test