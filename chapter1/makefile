CC = gcc // 컴파일러는 gcc를 사용한다
CFLAGS = // 컴파일 옵션은 없다
LDFLAGS = // 링크 옵션은 없다

.SUFFIXES : 
.c .o :
	$(CC) -c $(CFLAGS) $<
// .c 파일을 .o 파일로 컴파일하는 규칙을 정의한다

ALL = hello output input args string 
// ALL 변수에 컴파일할 파일을 저장한다

all : $(ALL) // all 명령어를 실행하면 ALL 변수에 저장된 파일을 컴파일한다

hello : hello.o // hello 파일을 컴파일한다
	$(CC) -o $@ hello.o // hello 파일을 링크한다

output : output.o // output 파일을 컴파일한다
	$(CC) -o $@ output.o // output 파일을 링크한다

input : input.o // input 파일을 컴파일한다
	$(CC) -o $@ input.o // input 파일을 링크한다

args: args.o
	$(CC) -o $@ $< $(LDFLAGS)
// args 파일을 컴파일하고 링크한다

string: string.o
	$(CC) -o $@ $< $(LDFLAGS)
// string 파일을 컴파일하고 링크한다

clean :
	rm -rf *.o $(ALL)
// clean 명령어를 실행하면 .o 파일과 ALL 변수에 저장된 파일을 삭제




