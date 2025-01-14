

#include <stdio.h>
// stdio.h는 printf, scanf, getchar, putchar, gets, puts 함수를 제공한다.


int main()
{
	char c, s[80];
	int i; long l;
	float f; double d;
	
	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d); 
// 	입력: a hello 100 99999 3.14 99.999
// s 앞에 &를 붙이지 않는 이유는 배열의 이름은 배열의 첫 번째 요소의 주소를 가리키기 때문이다.
	printf("Output: %c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d);
// 	Output: a hello 100 99999 3.1400 99.99 

	c = getchar();
	putchar(c);

	gets(s);
	puts(s);
}
// getchar 함수는 문자를 입력받는 함수, gets 함수는 문자열을 입력받는 함수이다
