
/*
세마포어(Semaphore)를 초기화하고,
 값을 설정하거나 변경하며, 동기화 작업을 수행하고 제거하는 함수들의 구현
*/

#include <stdio.h>      // 표준 입출력 함수 사용을 위한 헤더
#include "5_semlib.h"     // 세마포어 관련 함수 선언이 포함된 사용자 정의 헤더

// 세마포어를 초기화하고 생성하거나 기존 세마포어에 접근하는 함수
int
semInit(key_t key)
{
    int     semid;

    // 주어진 키로 세마포어 생성 또는 접근
    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  {
        perror("semget");  // 오류 발생 시 에러 메시지 출력
        return -1;         // 실패 시 -1 반환
    }

    return semid;          // 성공 시 세마포어 ID 반환
}

// 세마포어의 초기값을 설정하는 함수
int
semInitValue(int semid, int value)
{
    union semun {          // 세마포어 제어 명령에 사용할 공용체 정의
            int     val;   // 세마포어 값 설정을 위한 필드
    } semun;

    semun.val = value;     // 설정할 값 지정
    if (semctl(semid, 0, SETVAL, semun) < 0)  {  // 세마포어 값 설정
        perror("semctl");  // 오류 발생 시 에러 메시지 출력
        return -1;         // 실패 시 -1 반환
    }
 
    return semid;          // 성공 시 세마포어 ID 반환
}

// 세마포어 대기(P) 연산을 수행하는 함수 (값 감소)
int
semWait(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0;      // 대상 세마포어 번호 (단일 세마포어 사용)
    semcmd.sem_op = -1;      // 값을 감소시키는 연산 (-1)
    semcmd.sem_flg = SEM_UNDO;  // 프로세스 종료 시 자동 복구 플래그 설정
    if (semop(semid, &semcmd, 1) < 0)  {  // 연산 수행
        perror("semop");     // 오류 발생 시 에러 메시지 출력
        return -1;           // 실패 시 -1 반환
    }
 
    return 0;                // 성공 시 0 반환
}

// 비차단 대기(P) 연산을 수행하는 함수 (즉시 반환)
int
semTryWait(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0;                  // 대상 세마포어 번호 (단일 세마포어 사용)
    semcmd.sem_op = -1;                  // 값을 감소시키는 연산 (-1)
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO;  // 대기가 아닌 즉시 반환 플래그 설정
    if (semop(semid, &semcmd, 1) < 0)  {     // 연산 수행
        perror("semop");               // 오류 발생 시 에러 메시지 출력
        return -1;                     // 실패 시 -1 반환
    }
 
    return 0;                          // 성공 시 0 반환
}

// 세마포어 시그널(V) 연산을 수행하는 함수 (값 증가)
int
semPost(int semid)
{
    struct sembuf   semcmd;

    semcmd.sem_num = 0;      // 대상 세마포어 번호 (단일 세마포어 사용)
    semcmd.sem_op = 1;       // 값을 증가시키는 연산 (+1)
    semcmd.sem_flg = SEM_UNDO;  // 프로세스 종료 시 자동 복구 플래그 설정
    if (semop(semid, &semcmd, 1) < 0)  {  // 연산 수행
        perror("semop");     // 오류 발생 시 에러 메시지 출력
        return -1;           // 실패 시 -1 반환
    }
 
    return 0;                // 성공 시 0 반환
}

// 현재 세마포어 값을 조회하는 함수
int
semGetValue(int semid)
{
    union semun {          // 세마포어 제어 명령에 사용할 공용체 정의
            int     val;   // 더미 필드 (사용하지 않음)
    } dummy;

    return semctl(semid, 0, GETVAL, dummy);  // 현재 값 반환 (실패 시 음수 반환)
}

// 세마포어를 제거하는 함수 (자원 해제)
int
semDestroy(int semid)
{
    union semun {          // 세마포어 제어 명령에 사용할 공용체 정의
            int     val;   // 더미 필드 (사용하지 않음)
    } dummy;

    if (semctl(semid, 0, IPC_RMID, dummy) < 0) {  // 세마포어 제거 명령 실행
        perror("semctl");   // 오류 발생 시 에러 메시지 출력
        return -1;          // 실패 시 -1 반환
    }
    
    close(semid);           // 파일 디스크립터 닫기 (추가적인 자원 정리)
 
    return 0;               // 성공 시 0 반환
}
