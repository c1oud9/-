#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define	MAX_BUF	128

// 부모 프로세스가 자식 프로세스에게 무언가를 전달하는 예제

main()
{
	int		n, fd[2];
	pid_t	pid;
	char	buf[MAX_BUF];
// 부모 프로세스
	if (pipe(fd) < 0)  { // pipe 시스템 콜로 file desriptor 생성 
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) < 0)  { // 자식 프로세스
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		close(fd[1]); // 자식프로세스는 write를 사용하지 않으므로 닫아준다
		printf("Child : Wait for parent to send data\n");
		if ((n = read(fd[0], buf, MAX_BUF)) < 0)  { 
			//buf에 fd[0]에서 읽어온 데이터를 저장, MAX_BUFF만큼 읽어온다

			perror("read");
			exit(1)
		}
		printf("Child : Received data from parent: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n); //buf에 있는 데이터를 표준출력에 출력
	}
	else  {
		close(fd[0]); //부모프로세스는 read를 사용하지 않으므로 닫아준다
		strcpy(buf, "Hello, World!\n"); //buf에 데이터를 저장
		printf("Parent: Send data to child\n");
		write(fd[1], buf, strlen(buf)+1); 
		//buf에 있는 데이터를 fd[1]에 쓴다 +1은 NULL까지 쓰기위해

	}

	exit(0); //굳이 안써도 되지만 명시적으로 써이
}