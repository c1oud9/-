/*
세마포어(semaphore) 구현을 위한 함수들을 정의
POSIX 세마포어의 기본 기능을 제공합니다. 
뮤텍스와 조건 변수를 사용하여 세마포어의 동작을 구현하고 있으며, 
각 함수는 세마포어 연산의 원자성을 보장하기 위해 뮤텍스를 사용
*/
#include <stdio.h>
#include <pthread.h>
#include "13_semlib2.h"

// sem_init 함수: 세마포어 초기화
int sem_init(sem_t *sem, int pshared, int value)
{
    // pshared가 0이 아니면 지원하지 않는 기능으로 처리
    if (pshared)  {
        fprintf(stderr, "Function not supported\n");
        return -1;
    }

    // 뮤텍스 초기화
    if (pthread_mutex_init(&sem->mutex, NULL) < 0)
        return -1;

    // 조건 변수 초기화
    if (pthread_cond_init(&sem->cond, NULL) < 0)
        return -1;

    // 세마포어 값 설정
    sem->sval = value;

    return 0;
}

// sem_wait 함수: 세마포어 획득 (값이 0이면 대기)
int sem_wait(sem_t *sem)
{
    // 뮤텍스 잠금
    if (pthread_mutex_lock(&sem->mutex) < 0)
        return -1;

    // 세마포어 값이 0이면 대기
    if (sem->sval == 0)  {
        while (sem->sval == 0)  {
            // 조건 변수에서 대기
            if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0)  {
                if (pthread_mutex_unlock(&sem->mutex) < 0)
                    return -1;
                return -1;
            }
        }
        sem->sval--;
    }
    else  {
        sem->sval--;
    }

    // 뮤텍스 해제
    if (pthread_mutex_unlock(&sem->mutex) < 0)
        return -1;

    return 0;
}

// sem_trywait 함수: 세마포어 획득 시도 (대기하지 않음)
int sem_trywait(sem_t *sem)
{
    // 뮤텍스 잠금
    if (pthread_mutex_lock(&sem->mutex) < 0)
        return -1;

    // 세마포어 값이 0이면 즉시 반환
    if (sem->sval == 0)  {
        if (pthread_mutex_unlock(&sem->mutex) < 0)
            return -1;
        return -1;
    }
    else  {
        sem->sval--;
    }

    // 뮤텍스 해제
    if (pthread_mutex_unlock(&sem->mutex) < 0)
        return -1;

    return 0;
}

// sem_post 함수: 세마포어 반환
int sem_post(sem_t *sem)
{
    // 뮤텍스 잠금
    if (pthread_mutex_lock(&sem->mutex) < 0)
        return -1;

    // 세마포어 값이 0이면 대기 중인 스레드에 신호 전송
    if (sem->sval == 0)  {
        if (pthread_cond_signal(&sem->cond) < 0)  {
            if (pthread_mutex_unlock(&sem->mutex) < 0)
                return -1;
            return -1;
        }
    }

    // 세마포어 값 증가
    sem->sval++;

    // 뮤텍스 해제
    if (pthread_mutex_unlock(&sem->mutex) < 0)
        return -1;

    return 0;
}

// sem_getvalue 함수: 현재 세마포어 값 반환
int sem_getvalue(sem_t *sem, int *sval)
{
    *sval = sem->sval;
    return 0;
}

// sem_destroy 함수: 세마포어 제거
int sem_destroy(sem_t *sem)
{
    // 뮤텍스 제거
    if (pthread_mutex_destroy(&sem->mutex) < 0)
        return -1;

    // 조건 변수 제거
    if (pthread_cond_destroy(&sem->cond) < 0)
        return -1;

    return 0;
}
