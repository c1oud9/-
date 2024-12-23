/*
이 코드는 생산자-소비자 문제를 해결하기 위한 생산자(Producer) 프로그램의 일부입니다. 공유 메모리를 사용하여 유한 버퍼(Bounded Buffer)를 구현하고, 
생산자가 데이터를 생성하여 버퍼에 추가하는 과정을 보여줍니다
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

    srand(0x8888);  // 난수 생성기 초기화
    for (i = 0 ; i < NLOOPS ; i++)  {  // 정해진 횟수만큼 반복
        if (pBuf->counter == MAX_BUF)  {  // 버퍼가 가득 찼는지 확인
            printf("Producer: Buffer full. Waiting.....\n");
            while (pBuf->counter == MAX_BUF)  // 버퍼가 가득 찬 동안 대기
                ;
        }

        printf("Producer: Producing an item.....\n");
        data = (rand()%100)*10000;  // 랜덤 데이터 생성
        pBuf->buf[pBuf->in].data = data;  // 버퍼에 데이터 삽입
        pBuf->in = (pBuf->in + 1) % MAX_BUF;  // 다음 삽입 위치로 이동 (원형 버퍼)
        pBuf->counter++;  // 버퍼 내 아이템 수 증가

        usleep(data);  // 생성된 데이터 값만큼 대기 (마이크로초 단위)
    }

    printf("Producer: Produced %d items.....\n", i);  // 생산된 총 아이템 수 출력

    sleep(2);  // 2초 대기
    printf("Producer: %d items in buffer.....\n", pBuf->counter);  // 버퍼에 남아있는 아이템 수 출력
    
    // 공유 메모리 세그먼트 제거
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl");  // 오류 발생 시 에러 메시지 출력
        exit(1);  // 프로그램 종료
    }
}
