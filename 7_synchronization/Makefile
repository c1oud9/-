
CC = gcc 
#compiler는 gcc를 사용	

CFLAGS = 
#CFLAGS는 비어있음 = 컴파일 옵션은 없음

LDFLAGS = -lpthread
#LDFLAGS는 -lpthread = pthread 라이브러리를 링크, 


.SUFFIXES : .c .o
# .c 파일을 .o 파일로 만들기 위한 규칙을

.c.o :
	$(CC) -c $(CFLAGS) $<
# .c 파일을 .o 파일로 만들기 위한 규칙을 정의

ALL = producer consumer producer_s consumer_s prodcons dining dining2 prodcons_m prodcons_s
# ALL은 producer, consumer, producer_s, consumer_s, prodcons, dining, dining2, prodcons_m, prodcons_s로 정의

all: $(ALL)
# all은 타겟 이름, `$(ALL)'은 `ALL`의 값을 참조,
# 일반적으로 프로젝트에서 빌드해야 할 모든 주요 타겟들의 목록을 포함


prodcons_t: prodcons_t.o
	$(CC) -o $@ $< $(LDFLAGS)
# prodcons_t는 prodcons_t.o를 컴파일하여 만들어짐

producer: producer.o 
	$(CC) -o $@ $< $(LDFLAGS)
# producer는 producer.o를 컴파일하여 만들어짐

consumer: consumer.o 
	$(CC) -o $@ $< $(LDFLAGS)
# consumer는 consumer.o를 컴파일하여 만들어짐


producer_s: producer_s.o semlib.o
	$(CC) -o $@ $^ $(LDFLAGS)
# producer_s는 producer_s.o와 semlib.o를 컴파일하여 만들어짐

consumer_s: consumer_s.o semlib.o
	$(CC) -o $@ $^ $(LDFLAGS)
# consumer_s는 consumer_s.o와 semlib.o를 컴파일하여 만들어짐

prodcons: prodcons.o
	$(CC) -o $@ $< $(LDFLAGS)
# prodcons는 prodcons.o를 컴파일하여 만들어짐

dining: dining.o
	$(CC) -o $@ $< $(LDFLAGS)
# dining는 dining.o를 컴파일하여 만들어짐

dining2: dining2.o
	$(CC) -o $@ $< $(LDFLAGS)
# dining2는 dining2.o를 컴파일하여 만들어짐

prodcons_m: prodcons_m.o
	$(CC) -o $@ $< $(LDFLAGS)
# prodcons_m는 prodcons_m.o를 컴파일하여 만들어짐

prodcons_s: prodcons_s.o semlib2.o
	$(CC) -o $@ $^ $(LDFLAGS)
#	prodcons_s는 prodcons_s.o와 semlib2.o를 컴파일하여 만들어짐

clean :
	rm -rf *.o $(ALL)
# clean은 .o 파일과 $(ALL)을 삭제
