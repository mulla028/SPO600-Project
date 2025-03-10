BINARIES=hello
CCFLAGS=-g -O0 -fno-builtin -fdump-tree-amullagaliev

all: ${BINARIES}

hello: hello.c
	gcc ${CCFLAGS} -o hello hello.c

clean:
	rm ${BINARIES} *.o || true
