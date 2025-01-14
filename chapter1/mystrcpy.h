#include <stdio.h>

void mystrcpy(char *dst, char *src)
{
    while (*src)
    { 
        *dst++ = *src++;
    }
    *dst = *src;
}
/*
mystrcpy는 다음 기능을 가진다
while(*src)는 src가 NULL이 아닐 때까지 반복한다.
src는 포인터로 선언된 변수이기 때문에 기본값은 NULL이다.
*dst++ = *src++는 src의 값을 dst에 복사하고 src와 dst를 증가시킨다.
결국 mystrcpy는 src의 문자열을 dst에 복사하는 함수이다.

*/