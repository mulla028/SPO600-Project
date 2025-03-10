#include <stdio.h>

int foo(int p1, int p2) {
	return p1*p2;
}

int main() {
	int a = 12;
	int b = 13;

	int c;

	c = foo(a, b);

	printf("%d\n", c);

	return 0;
}
