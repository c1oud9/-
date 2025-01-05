#include <stdio.h>
// stdio.h는 printf 함수를 제공한다.


int main(int argc, char *argv[])
/* main 함수, 가장 먼저 호출되는 함수
main 함수는 두 개의 인자,argc, argv를 받는데,
argc는 명령행 인자의 개수를 나타내는 정수이다.
프로그램 이름도 첫번째 인자로 포함되기 때문에 최소값은 1이다.
argv는 명령행 인자들을 담고 있는 문자열 포인터 배열이다. 

*/

{
	int		i;

	for (i = 0 ; i < argc ; i++)
		printf("argv[%d]=%s\n", i, argv[i]);


}