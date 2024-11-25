#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"


main()
{
	int		shmid;
	char	*ptr, *pData;
	int		*pInt;


	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	pInt = (int *)ptr;
	pData = ptr + sizeof(int); 
    //처음 4바이트는 int형 변수이므로 데이터를 저장할 위치를 지정해주기 위해 4를 더해준다.

	sprintf(pData, "This is a request from %d.", getpid());
	*pInt = 1; //
	printf("Sent a request.....");

	while ((*pInt) == 1) // 다시  lcd1
		;

	printf("Received reply: %s\n", pData);
} 

//cpu가 1개라면 무한루프에 빠질 수 있다. 하지만 cpu protection이 있기 context swtiching이 이루어진다
