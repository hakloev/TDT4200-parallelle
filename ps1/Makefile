.PHONY: clean all 

START:=2
STOP:=10000000
PROCESSES:=4

all: parallel serial

parallel: computeMPI.c
	mpicc -g -Wall -o parallel computeMPI.c -std=c99 -lm -O2

run_parallel: parallel
	mpirun -n $(PROCESSES) ./parallel $(START) $(STOP) 

serial: computeSerial.c
	gcc -o $@ computeSerial.c -std=c99 -lm -O2

run_serial: serial
	./serial $(START) $(STOP)

clean:
	rm -f serial parallel
