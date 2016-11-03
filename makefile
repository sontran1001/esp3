example:	example.c fifo.c esp3.c fifo.h esp3.h 
	gcc -std=c99 example.c fifo.c esp3.c -o example
	
clean:
	rm -f example
