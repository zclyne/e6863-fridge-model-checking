# E6863 Project - Fridge Model Checking

This is the final project for Columbia University CSEE E6863 Formal Verification HW/SW System.
It aims at performing bounded model checking on Fridge, a multi-threaded key-value store that supports nonblocking and blocking GET operation, to help locating bugs that are hard to find via normal testing method.

This project utilizes [CBMC](http://www.cs.cmu.edu/~modelcheck/cbmc/) to perform model checking.

## Get Started

As `CBMC` requires adding code that cannot be normally compiled, the `master` branch of the repository is for running CBMC.

To run unit tests and e2e tests, you need to switch to the `test` tag using:
```shell
git checkout test
```

### Run Fridge

To run fridge, use the following `make` command to compile:

```shell
make app
```

and run the executable:

```shell
./app
```

### Run the unit tests

All the unit tests are located in `test/unit_test.c`.

To run the unit tests, use the following `make` command to compile:

```shell
make unit_test
```

and run the executable:

```shell
./unit_test
```

### Run the end-to-end tests

All the end-to-end tests are located in `test/e2e_test.c`.

To run the unit tests, use the following `make` command to compile:

```shell
make e2e_test
```

and run the executable:

```shell
./e2e_test
```

### Run CBMC Model Checking

#### Function-Level

We implemented function-level model checking for `kkv_init`, `kkv_destroy`, `kkv_get` and `kkv_put`.

To run CBMC on each function, you need to comment out the other three functions and then run the corresponding command in the `Makefile`.

TBD