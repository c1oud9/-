
CC = gcc
# compiler는 gcc 사용
CFLAGS =
# 컴파일 옵션은 디폴트 
LDFLAGS =
# 링크 옵션은 디폴트

.SUFFIXES : .c .o
# .c 파일을 .o 파일로 만들기 위한 규칙
.c.o :
	$(CC) -c $(CFLAGS) $<
# .c 파일을 .o 파일로 만드는 명령

ALL = order tcps tcpc udps udpc ucos ucoc ucls uclc tcpc_dns myusleep select sgs sgc
# ALL 변수에 컴파일할 파일들을 저장
all: $(ALL)
# all 명령어를 실행하면 ALL에 저장된 파일들을 컴파일

order: order.o 
	$(CC) -o $@ $< $(LDFLAGS)
# order.o 파일을 order로 컴파일

tcps: tcps.o 
	$(CC) -o $@ $< $(LDFLAGS)
# tcps.o 파일을 tcps로 컴파일

tcpc: tcpc.o 
	$(CC) -o $@ $< $(LDFLAGS)
# tcpc.o 파일을 tcpc로 컴파일

udps: udps.o 
	$(CC) -o $@ $< $(LDFLAGS)
# udps.o 파일을 udps로 컴파일

udpc: udpc.o 
	$(CC) -o $@ $< $(LDFLAGS)
# udpc.o 파일을 udpc로 컴파일

ucos: ucos.o 
	$(CC) -o $@ $< $(LDFLAGS)
# ucos.o 파일을 ucos로 컴파일

ucoc: ucoc.o 
	$(CC) -o $@ $< $(LDFLAGS)

ucls: ucls.o 
	$(CC) -o $@ $< $(LDFLAGS)

uclc: uclc.o 
	$(CC) -o $@ $< $(LDFLAGS)
# 

tcpc_dns: tcpc_dns.o 
	$(CC) -o $@ $< $(LDFLAGS)
# tcpc_dns.o 파일을 tcpc_dns로 컴파일

myusleep: myusleep.o 
	$(CC) -o $@ $< $(LDFLAGS)
# myusleep.o 파일을 myusleep로 컴파일

select: select.o 
	$(CC) -o $@ $< $(LDFLAGS)
# select.o 파일을 select로 컴파일

sgs: sgs.o 
	$(CC) -o $@ $< $(LDFLAGS)
# sgs.o 파일을 sgs로 컴파일

sgc: sgc.o 
	$(CC) -o $@ $< $(LDFLAGS)
# sgc.o 파일을 sgc로 컴파일

clean :
	rm -rf *.o $(ALL)
# clean은 .o 파일과 ALL에 저장된 파일들을 삭제하는 명령