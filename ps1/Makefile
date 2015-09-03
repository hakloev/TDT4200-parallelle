.PHONY: clean all 

CXX=icpc
CC=icc
FC=ifort

START:=2
STOP:=100000000
PROCESSES:=4

all: parallel serial

parallel: computeMPI.c
	mpicc -g -Wall -o parallel computeMPI.c -std=c99 -lm

run_parallel: parallel
	mpirun -np $(PROCESSES) ./parallel $(START) $(STOP) 

serial: computeSerial.c
	$(CC) -o $@ computeSerial.c -std=c99 -lm -O2

run_serial: serial
	./serial $(START) $(STOP)

clean:
	rm -f serial parallel
