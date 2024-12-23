
/*
이 코드는 생산자-소비자 문제의 소비자(Consumer) 프로그램 부분입니다.
 공유 메모리를 사용하여 유한 버퍼(Bounded Buffer)에서 데이터를 소비하는 과정을 구현합니다
*/
#include <stdio.h>      // 표준 입출력 함수를 사용하기 위한 헤더
#include <stdlib.h>     // 표준 라이브러리 함수를 사용하기 위한 헤더
#include <unistd.h>     // POSIX 운영체제 API를 사용하기 위한 헤더
#include <sys/types.h>  // 시스템 데이터 타입을 정의하는 헤더
#include <sys/ipc.h>    // IPC(프로세스 간 통신) 기능을 사용하기 위한 헤더
#include <sys/shm.h>    // 공유 메모리 기능을 사용하기 위한 헤더
#include "1_prodcons.h"   // 사용자 정의 헤더 파일 (버퍼 타입과 상수 정의)

main()
{
    BoundedBufferType   *pBuf;  // 공유 메모리에 매핑될 버퍼 포인터
    int                 shmid, i, data;  // 공유 메모리 ID, 루프 변수, 데이터 변수

    // 공유 메모리 세그먼트 접근
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");  // 오류 발생 시 에러 메시지 출력
        exit(1);  // 프로그램 종료
    }
    // 공유 메모리를 프로세스의 주소 공간에 부착
    if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat");  // 오류 발생 시 에러 메시지 출력
        exit(1);  // 프로그램 종료
    }

    srand(0x9999);  // 난수 생성기 초기화 (생산자와 다른 시드 사용)
    for (i = 0 ; i < NLOOPS ; i++)  {  // 정해진 횟수만큼 반복
        if (pBuf->counter == 0)  {  // 버퍼가 비어있는지 확인
            printf("Consumer: Buffer empty. Waiting.....\n");
            while (pBuf->counter == 0)  // 버퍼가 비어있는 동안 대기
                ;
        }

        printf("Consumer: Consuming an item.....\n");
        data = pBuf->buf[pBuf->out].data;  // 버퍼에서 데이터 추출
        pBuf->out = (pBuf->out + 1) % MAX_BUF;  // 다음 추출 위치로 이동 (원형 버퍼)
        pBuf->counter--;  // 버퍼 내 아이템 수 감소

        usleep((rand()%100)*10000);  // 랜덤한 시간 동안 대기 (마이크로초 단위)
    }

    printf("Consumer: Consumed %d items.....\n", i);  // 소비한 총 아이템 수 출력
    printf("Consumer: %d items in buffer.....\n", pBuf->counter);  // 버퍼에 남아있는 아이템 수 출력
}

/*
	1.	생산자가 생성한 공유 메모리 세그먼트에 접근하고 프로세스의 주소 공간에 부착합니다.
	2.	공유 버퍼에서 데이터를 추출하여 소비합니다.
	3.	버퍼가 비어있을 때 대기하는 로직을 구현합니다.
	4.	랜덤한 시간 동안 대기하여 소비 속도를 조절합니다.
	5.	모든 아이템을 소비한 후, 최종 상태를 출력합니다.
*/