// pointer에 대한 간단한 예제
#include <stdio.h>

int main() {
    int num = 10;
    int *ptr = &num;

    printf("num의 값: %d\n", num);
    printf("num의 주소: %p\n", &num); // 
    printf("ptr이 가리키는 주소: %p\n", ptr);
    printf("ptr이 가리키는 값: %d\n", *ptr);

    *ptr = 20;
    printf("변경 후 num의 값: %d\n", num);

    return 0;
}
