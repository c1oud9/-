
/*
코드의 목적
이 코드는 생산자-소비자 문제를 해결하기 위해 작성된 프로그램입니다.
`BoundedBufferType` 구조체를 사용하여 고정 크기의 순환 버퍼를 구현하고, 
두 개의 독립적인 스레드(생산자와 소비자)가 동시에 공유 자원(버퍼)을 
안전하게 사용하도록 설계되었습니다. `
ThreadUsleep()` 함수를 통해 랜덤한 시간 간격으로 작업이 수행됩니다.


*/


#include <stdio.h>      // 표준 입출력 함수 사용을 위한 헤더 파일
#include <stdlib.h>     // 표준 라이브러리 함수 사용을 위한 헤더 파일 (예: rand(), exit())
#include <unistd.h>     // 유닉스 표준 함수 사용을 위한 헤더 파일
#include <pthread.h>    // POSIX 스레드 관련 함수 사용을 위한 헤더 파일
#include <sys/time.h>   // 시간 측정을 위한 헤더 파일
#include "prodcons.h"   // 생산자-소비자 문제 관련 사용자 정의 헤더 파일

BoundedBufferType Buf;  // prodcons.h에 정의된 구조체 BoundedBufferType의 전역 변수 Buf 선언

// 특정 시간 동안 스레드를 대기시키는 함수
void ThreadUsleep(int usecs) {
    pthread_cond_t cond;          // 조건 변수 선언
    pthread_mutex_t mutex;        // 뮤텍스 선언
    struct timespec ts;           // 절대 시간 저장을 위한 구조체
    struct timeval tv;            // 현재 시간 저장을 위한 구조체

    // 조건 변수 초기화
    if (pthread_cond_init(&cond, NULL) < 0) {
        perror("pthread_cond_init");
        pthread_exit(NULL);       // 오류 발생 시 스레드 종료
    }

    // 뮤텍스 초기화
    if (pthread_mutex_init(&mutex, NULL) < 0) {
        perror("pthread_mutex_init");
        pthread_exit(NULL);
    }

    gettimeofday(&tv, NULL);      // 현재 시간을 가져옴
    ts.tv_sec = tv.tv_sec + usecs / 1000000;  // 초 단위로 변환하여 추가
    ts.tv_nsec = (tv.tv_usec + (usecs % 1000000)) * 1000;  // 나머지 마이크로초를 나노초로 변환하여 추가

    if (ts.tv_nsec >= 1000000000) {  // 나노초가 1초 이상이면 조정
        ts.tv_nsec -= 1000000000;
        ts.tv_sec++;
    }

    if (pthread_mutex_lock(&mutex) < 0) {  // 뮤텍스 잠금 시도
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0) {  // 조건 변수 대기 (지정된 시간까지)
        perror("pthread_cond_timedwait");
        pthread_exit(NULL);
    }

    if (pthread_cond_destroy(&cond) < 0) {  // 조건 변수 해제
        perror("pthread_cond_destroy");
        pthread_exit(NULL);
    }

    if (pthread_mutex_destroy(&mutex) < 0) {  // 뮤텍스 해제
        perror("pthread_mutex_destroy");
        pthread_exit(NULL);
    }
}

// 생산자 스레드 함수
void Producer(void *dummy) {
    int i, data;

    printf("Producer: Start.....\n");

    for (i = 0; i < NLOOPS; i++) {          // NLOOPS만큼 반복하여 아이템 생성
        if (Buf.counter == MAX_BUF) {       // 버퍼가 가득 찬 경우 대기
            printf("Producer: Buffer full. Waiting.....\n");
            while (Buf.counter == MAX_BUF); // 버퍼가 비워질 때까지 대기
        }

        printf("Producer: Producing an item.....\n");
        data = (rand() % 100) * 10000;      // 랜덤 데이터 생성
        Buf.buf[Buf.in].data = data;        // 데이터를 버퍼에 저장
        Buf.in = (Buf.in + 1) % MAX_BUF;    // 다음 저장 위치 계산 (순환 버퍼)
        Buf.counter++;                      // 버퍼 내 아이템 개수 증가

        ThreadUsleep(data);                 // 랜덤 시간만큼 대기
    }

    printf("Producer: Produced %d items.....\n", i);
    printf("Producer: %d items in buffer.....\n", Buf.counter);

    pthread_exit(NULL);                     // 스레드 종료
}

// 소비자 스레드 함수
void Consumer(void *dummy) {
    int i, data;

    printf("Consumer: Start.....\n");

    for (i = 0; i < NLOOPS; i++) {          // NLOOPS만큼 반복하여 아이템 소비
        if (Buf.counter == 0) {             // 버퍼가 비어 있는 경우 대기
            printf("Consumer: Buffer empty. Waiting.....\n");
            while (Buf.counter == 0);       // 버퍼에 아이템이 추가될 때까지 대기
        }

        printf("Consumer: Consuming an item.....\n");
        data = Buf.buf[Buf.out].data;       // 데이터를 버퍼에서 가져옴
        Buf.out = (Buf.out + 1) % MAX_BUF;  // 다음 읽기 위치 계산 (순환 버퍼)
        Buf.counter--;                      // 버퍼 내 아이템 개수 감소

        ThreadUsleep((rand() % 100) * 10000);   // 랜덤 시간만큼 대기
    }

    printf("Consumer: Consumed %d items.....\n", i);
    printf("Consumer: %d items in buffer.....\n", Buf.counter);

    pthread_exit(NULL);                     // 스레드 종료
}

// main 함수: 생산자와 소비자 스레드를 생성하고 실행 관리
int main() {
    pthread_t tid1, tid2;

    srand(0x8888);                          // 랜덤 시드 설정

    if (pthread_create(&tid1, NULL, (void *)Producer, NULL) < 0) {   // 생산자 스레드 생성
        perror("pthread_create");
        exit(1);
    }

    if (pthread_create(&tid2, NULL, (void *)Consumer, NULL) < 0) {   // 소비자 스레드 생성
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(tid1, NULL) < 0) {     // 생산자 스레드 종료 대기 및 자원 정리
        perror("pthread_join");
        exit(1);
    }
    
    if (pthread_join(tid2, NULL) < 0) {     // 소비자 스레드 종료 대기 및 자원 정리
        perror("pthread_join");
        exit(1);
    }

    printf("Main : %d items in buffer.....\n", Buf.counter);   // 최종 버퍼 상태 출력

    return 0;
}
