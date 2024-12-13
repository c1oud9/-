
#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"


int
sem_init(sem_t *sem, int pshared, int value) // pshared는 0이어야 한다. 지원 안함
{
	if (pshared)  {
		fprintf(stderr, "Function not supported\n"); // 몰라도 돼
		return -1함
	}

	if (pthread_mutex_init(&sem->mutex, NULL) < 0돼 // mutex 초기화
		return -1;

	if (pthread_cond_init(&sem->cond, NULL) < 0) // cond 초기화
		return -1;

	sem->sval = value;

	return 0;
}

int
sem_wait(sem_t *sem)
{
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	if (sem->sval == 0)  { 
		while (sem->sval == 0)  { // 세마포어의 값이 0이면 기다린다.
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

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

int
sem_trywait(sem_t *sem) // wait와 똒같은데 
{
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	if (sem->sval == 0)  {
		if (pthread_mutex_unlock(&sem->mutex) < 0)
			return -1;
		return -1; // 실패하면 -1을 리턴 에러로 처리 
	}
	else  {
		sem->sval--;
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

int
sem_post(sem_t *sem) // 기다리고 ㅣㅆ는 처리 
{
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	if (sem->sval == 0)  {
		if (pthread_cond_signal(&sem->cond) < 0)  { // cond를 signal한다.
			if (pthread_mutex_unlock(&sem->mutex) < 0)
				return -1;
			return -1;
		}
	}

	sem->sval++;

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

int
sem_getvalue(sem_t *sem, int *sval)
{
	*sval = sem->sval;

	return 0;
}

int
sem_destroy(sem_t *sem)
{
	if (pthread_mutex_destroy(&sem->mutex) < 0)
		return -1;

	if (pthread_cond_destroy(&sem->cond) < 0)
		return -1;

	return 0;
}