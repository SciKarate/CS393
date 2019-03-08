CXX = c++
CFLAGS = -g -std=c++17 -Wall -pedantic

all: testDevice myShell

.cc.o:
	$(CXX) -c $(CFLAGS) $<

clean:
	rm -f *.o *.dev

spotless: clean
	rm -f testDevice myShell

TEST_DEV_OBJS = block_device.o test_device.o
SHELL_OBJS = my_shell.o

testDevice: $(TEST_DEV_OBJS)
	$(CXX) -o testDevice $(TEST_DEV_OBJS)

myShell: $(SHELL_OBJS)
	$(CXX) -o myShell -lreadline $(SHELL_OBJS)

test: all
	./testDevice
