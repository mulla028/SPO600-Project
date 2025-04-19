CC = ~/gcc-build-001/gcc/xgcc
CFLAGS = -B ~/gcc-build-001/gcc/ -g -O3 -ftree-vectorize -fdump-tree-amullagaliev

all: complex-test

complex-test: complex-clones-test.c
	$(CC) $(CFLAGS) complex-clones-test.c -o complex-test

run-test: complex-test
	./complex-test
	@echo "Test completed. Check the dump files for analysis results."

show-results:
	@echo "====== Analysis Results ======"
	@grep -A 3 -B 1 "PRUNE\\|NOPRUNE" complex-test-complex-clones-test.c.*.amullagaliev || echo "No results found"
	@echo ""
	@echo "====== Summary of PRUNE/NOPRUNE decisions ======"
	@grep "PRUNE:" complex-test-complex-clones-test.c.*.amullagaliev | sort | uniq
	@grep "NOPRUNE:" complex-test-complex-clones-test.c.*.amullagaliev | sort | uniq

clean:
	rm -f complex-test *.o *.amullagaliev*

.PHONY: all run-test show-results clean
