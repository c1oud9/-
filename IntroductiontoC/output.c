
#include <stdio.h>
// stdio.h는 printf, putchar, puts 함수를 제공한다.


main()
{
	char c = 'a', s[] = "hello";
	int i = 100; long l = 99999;
	float f = 3.14; double d = 99.999;
	int *p = &i;
	
	printf("Output: %c %s %d %#X %ld %.4f %.2lf %p\n", c, s, i, i, l, f, d, p);
    // Output: a hello 100 0x64 99999 3.1400 99.99 0x7fff5fbff7c4
    // %c: char, %s: string, %d: int, %#X: int(hex), %ld: long, %.4f: float(소수점 4자리까지), %.2lf: double(소수점 2자리까지), %p: pointer  
	putchar(c);
	puts(s);
}