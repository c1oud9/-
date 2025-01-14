#include <stdio.h>
// stdio.h는 printf 함수를 제공한다.

int main(int argc, char *argv[])

{
	printf ("argc = %d\n", argc);
	printf ("argv[0] = %s\n", argv[1]);


}

/*
c언어 프로그램에서 main 함수는 2개의 매개변수를 가진다 
명령행이란 프로그램을 실행할 때 프로그램 이름 뒤에 입력하는 문자열을 말한다.
argc는 명령행에 입력된 문자열의 개수를 나타내는 정수이다.
argc의 디폴트 값은 1이다. 그이유는 프로그램 이름이 argc에 포함되기 때문이다이
argv는 명령행에 입력된 문자열을 나타내는 포인터 배열이다.
따라서 argv[0]은 프로그램 이름을 나타낸다.

*/