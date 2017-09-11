CC = gcc
ifeq ($(RELEASE),1)
CFLAGS = -O2 -DNDEBUG -c
LDFLAGS = -O2 -DNDEBUG -o
else
CFLAGS = -c -g -O0 -DDEBUG -W -Wall
LDFLAGS = -O0 -DDEBUG -W -Wall -o
endif
SRCS = $(notdir $(wildcard *.c))
OBJS = $(SRCS:.c=.o)
TARGET = BlankLearner
 
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $<

run:
	.\$(TARGET)

clean:
	del *.o $(TARGET)