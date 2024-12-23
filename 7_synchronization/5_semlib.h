/*
이 코드는 세마포어(Semaphore) 연산을 위한 함수 선언과 필요한 헤더 파일을 포함하고 있습니다. 
세마포어는 프로세스 간 동기화를 위해 사용되는 중요한 IPC(Inter-Process Communication) 메커니즘

*/


#include <sys/types.h>  // 시스템 데이터 타입을 정의하는 헤더
#include <sys/ipc.h>    // IPC(프로세스 간 통신) 기능을 사용하기 위한 헤더
#include <sys/sem.h>    // 세마포어 기능을 사용하기 위한 헤더

// 세마포어 초기화 함수: 주어진 키로 세마포어 생성 또는 접근
int     semInit(key_t key);

// 세마포어 초기값 설정 함수: 지정된 세마포어 ID의 값을 설정
int     semInitValue(int semid, int value);

// 세마포어 대기 함수: 세마포어 값을 감소시키고, 0이면 대기
int     semWait(int semid);

// 세마포어 비차단 대기 함수: 세마포어 값을 감소시키되, 대기하지 않고 즉시 반환
int     semTryWait(int semid);

// 세마포어 시그널 함수: 세마포어 값을 증가시키고 대기 중인 프로세스 깨움
int     semPost(int semid);

// 세마포어 값 조회 함수: 현재 세마포어 값을 반환
int     semGetValue(int semid);

// 세마포어 제거 함수: 지정된 세마포어를 시스템에서 제거
int     semDestroy(int semid);
