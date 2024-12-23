/*
세마포어를 사용한 생산자-소비자 문제의 다중 스레드 해결 방법을 구현
1.	EmptySem (빈 버퍼 세마포어)
	•	초기값: `MAX_BUF`
	•	생산자가 버퍼에 공간이 있는지 확인
	•	버퍼가 가득 찼을 때 생산자 대기
2.	FullSem (찬 버퍼 세마포어)
	•	초기값: `0`
	•	소비자가 소비할 아이템이 있는지 확인
	•	버퍼가 비어있을 때 소비자 대기
3.	MutexSem (상호 배제 세마포어)
	•	초기값: `1`
	•	버퍼에 대한 동시 접근 방지
	•	한 번에 하나의 스레드만 버퍼 조작 가능
*/

#include <stdio.h>      // 표준 입출력 함수 헤더
#include <stdlib.h>     // 표준 라이브러리 함수 헤더
#include <unistd.h>     // POSIX 운영체제 API 헤더
#include <pthread.h>    // 스레드 관련 함수 헤더
#include <sys/time.h>   // 시간 관련 함수 헤더
#include "semlib2.h"    // 사용자 정의 세마포어 라이브러리 헤더
#include "prodcons.h"   // 생산자-소비자 관련 헤더

// 공유 버퍼와 세마포어 전역 변수 선언
BoundedBufferType	Buf;  // 유한 버퍼
sem_t				EmptySem, FullSem, MutexSem;  // 세마포어들

// 정밀한 스레드 대기 함수 (마이크로초 단위)
void ThreadUsleep(int usecs)
{
    // pthread 조건 변수와 뮤텍스를 사용한 정밀 대기 구현
    pthread_cond_t		cond;
    pthread_mutex_t		mutex;
    struct timespec		ts;
    struct timeval		tv;

    // 조건 변수와 뮤텍스 초기화
    if (pthread_cond_init(&cond, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    if (pthread_mutex_init(&mutex, NULL) < 0)  {
        perror("pthread_mutex_init");
        pthread_exit(NULL);
    }

    // 대기 시간 계산
    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec + usecs/1000000;
    ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
    if (ts.tv_nsec >= 1000000000)  {
        ts.tv_nsec -= 1000000000;
        ts.tv_sec++;
    }

    // 조건 변수를 사용한 정밀 대기
    if (pthread_mutex_lock(&mutex) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }
    if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
        perror("pthread_cond_timedwait");
        pthread_exit(NULL);
    }

    // 자원 정리
    if (pthread_cond_destroy(&cond) < 0)  {
        perror("pthread_cond_destroy");
        pthread_exit(NULL);
    }
    if (pthread_mutex_destroy(&mutex) < 0)  {
        perror("pthread_mutex_destroy");
        pthread_exit(NULL);
    }
}

// 생산자 스레드 함수
void Producer(void *dummy)
{
    int		i, data;

    printf("Producer: Start.....\n");

    for (i = 0 ; i < NLOOPS ; i++)  {
        // 빈 버퍼 세마포어 대기 (버퍼에 공간 확보)
        if (sem_wait(&EmptySem) < 0)  {
            perror("sem_wait");
            pthread_exit(NULL);
        }
        // 뮤텍스 세마포어로 임계 구역 진입
        if (sem_wait(&MutexSem) < 0)  {
            perror("sem_wait");
            pthread_exit(NULL);
        }

        printf("Producer: Producing an item.....\n");
        // 랜덤 데이터 생성 및 버퍼에 삽입
        data = (rand()%100)*10000;
        Buf.buf[Buf.in].data = data;
        Buf.in = (Buf.in + 1) % MAX_BUF;
        Buf.counter++;

        // 뮤텍스와 찬 버퍼 세마포어 해제
        if (sem_post(&MutexSem) < 0)  {
            perror("sem_post");
            pthread_exit(NULL);
        }
        if (sem_post(&FullSem) < 0)  {
            perror("sem_post");
            pthread_exit(NULL);
        }

        // 생성된 데이터 값만큼 대기
        ThreadUsleep(data);
    }

    // 생산 결과 출력
    printf("Producer: Produced %d items.....\n", i);
    printf("Producer: %d items in buffer.....\n", Buf.counter);

    pthread_exit(NULL);
}

// 소비자 스레드 함수
void Consumer(void *dummy)
{
    int		i, data;

    printf("Consumer: Start.....\n");

    for (i = 0 ; i < NLOOPS ; i++)  {
        // 찬 버퍼 세마포어 대기 (소비할 아이템 확보)
        if (sem_wait(&FullSem) < 0)  {
            perror("sem_wait");
            pthread_exit(NULL);
        }
        // 뮤텍스 세마포어로 임계 구역 진입
        if (sem_wait(&MutexSem) < 0)  {
            perror("sem_wait");
            pthread_exit(NULL);
        }

        printf("Consumer: Consuming an item.....\n");
        // 버퍼에서 데이터 추출
        data = Buf.buf[Buf.out].data;
        Buf.out = (Buf.out + 1) % MAX_BUF;
        Buf.counter--;

        // 뮤텍스와 빈 버퍼 세마포어 해제
        if (sem_post(&MutexSem) < 0)  {
            perror("sem_post");
            pthread_exit(NULL);
        }
        if (sem_post(&EmptySem) < 0)  {
            perror("sem_post");
            pthread_exit(NULL);
        }

        // 랜덤 시간 대기
        ThreadUsleep((rand()%100)*10000);
    }

    // 소비 결과 출력
    printf("Consumer: Consumed %d items.....\n", i);
    printf("Consumer: %d items in buffer.....\n", Buf.counter);

    pthread_exit(NULL);
}

main()
{
    pthread_t	tid1, tid2;  // 스레드 ID 변수

    // 랜덤 시드 설정
    srand(0x8888);

    // 세마포어 초기화
    // EmptySem: 버퍼의 빈 공간 (최대 버퍼 크기로 초기화)
    if (sem_init(&EmptySem, 0, MAX_BUF) < 0)  {
        perror("sem_init");
        exit(1);
    }
    // FullSem: 찬 버퍼 (초기에는 0)
    if (sem_init(&FullSem, 0, 0) < 0)  {
        perror("sem_init");
        exit(1);
    }
    // MutexSem: 상호 배제 세마포어 (1로 초기화)
    if (sem_init(&MutexSem, 0, 1) < 0)  {
        perror("sem_init");
        exit(1);
    }

    // 생산자와 소비자 스레드 생성
    if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }
    if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

    // 스레드 종료 대기
    if (pthread_join(tid1, NULL) < 0)  {
        perror("pthread_join");
        exit(1);
    }
    if (pthread_join(tid2, NULL) < 0)  {
        perror("pthread_join");
        exit(1);
    }
    
    // 최종 버퍼 상태 출력
    printf("Main    : %d items in buffer.....\n", Buf.counter);

    // 세마포어 제거
    if (sem_destroy(&EmptySem) < 0)  {
        perror("sem_destroy");
    }
    if (sem_destroy(&FullSem) < 0)  {
        perror("sem_destroy");
    }
    if (sem_destroy(&MutexSem) < 0)  {
        perror("sem_destroy");
    }
}
