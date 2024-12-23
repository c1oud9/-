
/*
생산자-소비자 문제를 세마포어와 공유 메모리를 사용하여 해결하는 생산자 프로그램입니다
*/
#include <stdio.h>      // 표준 입출력 함수를 사용하기 위한 헤더
#include <stdlib.h>     // 표준 라이브러리 함수를 사용하기 위한 헤더
#include <unistd.h>     // POSIX 운영체제 API를 사용하기 위한 헤더
#include <sys/types.h>  // 시스템 데이터 타입을 정의하는 헤더
#include <sys/ipc.h>    // IPC(프로세스 간 통신) 기능을 사용하기 위한 헤더
#include <sys/shm.h>    // 공유 메모리 기능을 사용하기 위한 헤더
#include "semlib.h"     // 세마포어 라이브러리 헤더
#include "prodcons.h"   // 생산자-소비자 관련 헤더

main()
{
    BoundedBufferType   *pBuf;  // 공유 메모리에 매핑될 버퍼 포인터
    int                 shmid, i, data;  // 공유 메모리 ID, 루프 변수, 데이터 변수
    int                 emptySemid, fullSemid, mutexSemid;  // 세마포어 ID들

    // 공유 메모리 세그먼트 생성 또는 접근
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");  // 오류 발생 시 에러 메시지 출력
        exit(1);  // 프로그램 종료
    }
    // 공유 메모리를 프로세스의 주소 공간에 부착
    if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat");  // 오류 발생 시 에러 메시지 출력
        exit(1);  // 프로그램 종료
    }

    // 세 가지 세마포어 초기화
    // emptySemid: 빈 버퍼 세마포어
    if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }
    // fullSemid: 찬 버퍼 세마포어
    if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }
    // mutexSemid: 상호 배제(Mutex) 세마포어
    if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }

    srand(0x8888);  // 난수 생성기 초기화
    for (i = 0 ; i < NLOOPS ; i++)  {  // 정해진 횟수만큼 반복
        // 빈 버퍼 세마포어 대기 (버퍼에 공간이 있을 때까지)
        if (semWait(emptySemid) < 0)  {
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }
        // 상호 배제 세마포어 대기 (임계 구역 진입)
        if (semWait(mutexSemid) < 0)  {
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }

        printf("Producer: Producing an item.....\n");
        data = (rand()%100)*10000;  // 랜덤 데이터 생성
        pBuf->buf[pBuf->in].data = data;  // 버퍼에 데이터 삽입
        pBuf->in = (pBuf->in + 1) % MAX_BUF;  // 다음 삽입 위치로 이동 (원형 버퍼)
        pBuf->counter++;  // 버퍼 내 아이템 수 증가

        // 상호 배제 세마포어 해제 (임계 구역 탈출)
        if (semPost(mutexSemid) < 0)  {
            fprintf(stderr, "semPost failure\n");
            exit(1);
        }
        // 찬 버퍼 세마포어 시그널 (소비자에게 알림)
        if (semPost(fullSemid) < 0)  {
            fprintf(stderr, "semPost failure\n");
            exit(1);
        }
        usleep(data);  // 생성된 데이터 값만큼 대기
    }

    printf("Producer: Produced %d items.....\n", i);  // 생산된 총 아이템 수 출력

    sleep(2);  // 2초 대기
    printf("Producer: %d items in buffer.....\n", pBuf->counter);  // 버퍼에 남아있는 아이템 수 출력
    
    // 세마포어 제거
    if (semDestroy(emptySemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    if (semDestroy(fullSemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    if (semDestroy(mutexSemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    // 공유 메모리 세그먼트 제거
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl");
        exit(1);
    }
}