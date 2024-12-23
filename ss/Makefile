CC = gcc
#컴파일러는 gcc를 사용한다. 
CFLAGS =
#컴파일 옵션은 없다
LDFLAGS = -lsocket -lnsl -lpthread
#링커 옵션은  socket, nsl, pthread를 사용한다

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<
#컴파일러로 .c 파일을 .o 파일로 만든다.

ALL = chats chatc
#ALL이라는 변수에 chats와 chatc를 저장한다.

all: $(ALL)
#all이라는 명령어를 실행하면 $(ALL)을 실행한다.

chats: chats.o 
	$(CC) -o $@ $< $(LDFLAGS)
#chats를 만들기 위해서는 chats.o가 필요하다. chats.o를 링크하여 chats를 만든다.
chatc: chatc.o 
	$(CC) -o $@ $< $(LDFLAGS)
#chatc를 만들기 위해서는 chatc.o가 필요하다. chatc.o를 링크하여 chatc를 만든다.

clean :
	rm -rf *.o $(ALL)
#	.o 파일과 $(ALL)을 삭제한다.
