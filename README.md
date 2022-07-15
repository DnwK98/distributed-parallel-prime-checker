# Distributed Parallel Prime Checker

## Task

Check if given integer is prime by checking if it has divisors. 

The number should be represented by an array of digits.

## Run
```bash
# Sequential
make sequential number={number_to_check}
```
```bash
# OpenMP
make openmp number={number_to_check}
```
```bash
# Threads
make threads number={number_to_check}
```
```bash
# Message Passing Interface
make mpi number={number_to_check}
```
```bash
# Remote Procedure Call
make rpc number={number_to_check}
```
