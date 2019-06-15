all: sw-render sw-render.omp

clean:
	rm -f sw-render sw-render.omp

CFLAGS ?= -Wall -O2
LDFLAGS ?= -lm

sw-render: sw-render.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

sw-render.omp: sw-render.c
	$(CC) $(CFLAGS) -fopenmp -o $@ $< $(LDFLAGS)
