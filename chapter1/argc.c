#include <stdio.h>
// stdio.h는 printf 함수를 제공한다.


int main(int argc, char *argv[])
{
	int		i;

	for (i = 0 ; i < argc ; i++)
		printf("argv[%d]=%s\n", i, argv[i]);
}