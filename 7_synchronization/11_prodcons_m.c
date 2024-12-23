/*
NotFull: 버퍼에 공간이 있음을 알리는 조건 변수
	•	생산자가 버퍼에 데이터를 추가할 수 있는지 확인
	•	버퍼가 가득 찼을 때 생산자 대기
NotEmpty: 버퍼에 아이템이 있음을 알리는 조건 변수
	•	소비자가 버퍼에서 데이터를 소비할 수 있는지 확인
	•	버퍼가 비어있을 때 소비자 대기
*/

#include <stdio.h>      // 표준 입출력 함수를 위한 헤더
#include <stdlib.h>     // 표준 라이브러리 함수를 위한 헤더
#include <unistd.h>     // POSIX 운영체제 API를 위한 헤더
#include <pthread.h>    // 스레드 관련 함수를 위한 헤더
#include <sys/time.h>   // 시간 관련 함수를 위한 헤더
#include "1_prodcons.h"   // 생산자-소비자 관련 헤더

// 공유 자원: d유한 버퍼
BoundedBufferType	Buf;

// 동기화 도구
pthread_cond_t		NotFull;   // 버퍼가 가득 차지 않았음을 알리는 조건 변수
pthread_cond_t		NotEmpty;  // 버퍼가 비어있지 않음을 알리는 조건 변수
pthread_mutex_t		Mutex;     // 임계 구역 보호를 위한 뮤텍스

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
        // 뮤텍스 잠금 (임계 구역 진입)
        if (pthread_mutex_lock(&Mutex) < 0)  {
            perror("pthread_mutex_lock");
            pthread_exit(NULL);
        }

        // 버퍼가 가득 찼을 경우 대기
        while (Buf.counter == MAX_BUF)  {
            if (pthread_cond_wait(&NotFull, &Mutex) < 0)  {
                perror("pthread_cond_wait");
                pthread_exit(NULL);
            }
        }

        // 아이템 생산
        printf("Producer: Producing an item.....\n");
        data = (rand()%100)*10000;
        Buf.buf[Buf.in].data = data;
        Buf.in = (Buf.in + 1) % MAX_BUF;
        Buf.counter++;

        // 버퍼에 아이템이 있음을 알림
        if (pthread_cond_signal(&NotEmpty) < 0)  {
            perror("pthread_cond_signal");
            pthread_exit(NULL);
        }
        
        // 뮤텍스 해제
        if (pthread_mutex_unlock(&Mutex) < 0)  {
            perror("pthread_mutex_unlock");
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
        // 뮤텍스 잠금 (임계 구역 진입)
        if (pthread_mutex_lock(&Mutex) < 0)  {
            perror("pthread_mutex_lock");
            pthread_exit(NULL);
        }

        // 버퍼가 비어있을 경우 대기
        while (Buf.counter == 0)  {
            if (pthread_cond_wait(&NotEmpty, &Mutex) < 0)  {
                perror("pthread_cond_wait");
                pthread_exit(NULL);
            }
        }

        // 아이템 소비
        printf("Consumer: Consuming an item.....\n");
        data = Buf.buf[Buf.out].data;
        Buf.out = (Buf.out + 1) % MAX_BUF;
        Buf.counter--;

        // 버퍼에 공간이 있음을 알림
        if (pthread_cond_signal(&NotFull) < 0)  {
            perror("pthread_cond_signal");
            pthread_exit(NULL);
        }
        
        // 뮤텍스 해제
        if (pthread_mutex_unlock(&Mutex) < 0)  {
            perror("pthread_mutex_unlock");
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

    // 조건 변수 초기화
    if (pthread_cond_init(&NotFull, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    if (pthread_cond_init(&NotEmpty, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    
    // 뮤텍스 초기화
    if (pthread_mutex_init(&Mutex, NULL) < 0)  {
        perror("pthread_mutex_init");
        pthread_exit(NULL);
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

    // 동기화 도구 제거
    if (pthread_cond_destroy(&NotFull) < 0)  {
        perror("pthread_cond_destroy");
    }
    if (pthread_cond_destroy(&NotEmpty) < 0)  {
        perror("pthread_cond_destroy");
    }
    if (pthread_mutex_destroy(&Mutex) < 0)  {
        perror("pthread_mutex_destroy");
    }
}
