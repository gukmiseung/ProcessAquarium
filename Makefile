CC = gcc
CPPFLAGS += -Iinclude -D_POSIX_C_SOURCE=200809L
CFLAGS += -std=c11 -Wall -Wextra -Wpedantic -g -pthread
LDLIBS += -pthread
UI_LIBS = -lncursesw
BUILD = build
MONITOR_SRC = src/monitor.c
SCHEDULER_SRC = src/scheduler.c src/output.c
UI_SRC = src/ui.c
HEADERS = $(wildcard include/*.h)

.PHONY: all clean test test-ui test-monitor test-scheduler
all: aquarium

aquarium: src/main.c $(MONITOR_SRC) $(SCHEDULER_SRC) $(UI_SRC) $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(filter %.c,$^) -o $@ $(LDLIBS) $(UI_LIBS)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/test_ui: tests/test_ui.c tests/mock_backend.c $(UI_SRC) $(HEADERS) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(filter %.c,$^) -o $@ $(LDLIBS) $(UI_LIBS)

$(BUILD)/test_monitor: tests/test_monitor.c $(MONITOR_SRC) $(HEADERS) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(filter %.c,$^) -o $@ $(LDLIBS)

$(BUILD)/test_scheduler: tests/test_scheduler.c $(SCHEDULER_SRC) $(HEADERS) | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(filter %.c,$^) -o $@ $(LDLIBS)

test-ui: $(BUILD)/test_ui
	./$(BUILD)/test_ui

test-monitor: $(BUILD)/test_monitor
	./$(BUILD)/test_monitor

test-scheduler: $(BUILD)/test_scheduler
	./$(BUILD)/test_scheduler

test: test-monitor test-scheduler

clean:
	rm -rf $(BUILD) aquarium
