#include <stdio.h>
#include "mystrcpy.h"
int main ()
{
    char str[80];

    mystrcpy(str, "Hello, World!");
    puts(str);
}
/*
위 코드는 mystrcpy.h를 사용하여 문자열을 복사하는 코드이다.
puts(str)은 str에 저장된 문자열을 출력하는 함수이다.
*/
