Hey! Welcome to my filesystem!

If you want to try the real deal out,
> g++ block_device.cc my_shell.cpp -o shell.out -lreadline
> ./shell.out

Type "help" once the shell is open to view all the commands (I've added some of my own.)

If you want to see some testing, try

> g++ block_device.cc test_device.cc -o test.out
> ./test.out

You can also try
> python3 testerX.py | ./shell.out
tester1 tests a filesystem where blockcount < bytesperblock
	it expects 10111010
tester2 tests a filesystem where blockcount > bytesperblock
	it expects a string of ones, followed by 4 dispersed 1s
tester3 tests a filesystem where blockcount = bytesperblock
	it expects a huge string of 1s with a 0, followed by 4 dispersed 1s

If you want to test iNodes, try tester4, tester5, and tester 6.
tester 4 tests a basic iNodeMap where blockcount = bytesperblock.
tester 5 tests iNodeMaps where blockcount nor bpp are divisible by 8.
tester 6 tests an iNode map where blockcount > bytesperblock.

./test.out should pass valgrind perfectly.
./shell.out might result in some "possibly lost" blocks.