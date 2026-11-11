CC       ?= gcc
CFLAGS   := -std=c11 -Wall -Wextra -O2 -g -I. -pthread -MMD -MP
LDFLAGS  := -pthread

APP_SRC  := monitor.c producer.c consumer.c main.c
TEST_SRC := monitor.c producer.c consumer.c test_monitor.c
APP_OBJ  := $(APP_SRC:.c=.o)
TEST_OBJ := $(TEST_SRC:.c=.o)

all: app

app: $(APP_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

test_monitor: $(TEST_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: test_monitor
	./test_monitor | tail -1

run: app
	./app | tail -5

valgrind: app
	valgrind --leak-check=full --error-exitcode=1 ./app 2>&1 | tail -20

clean:
	rm -f app test_monitor *.o *.d

-include $(APP_OBJ:.o=.d) $(TEST_OBJ:.o=.d)

.PHONY: all test run valgrind clean
