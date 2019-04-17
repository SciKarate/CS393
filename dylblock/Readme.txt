By Dylan McNamee

My implementation of Assignment 3 - the inode map & inode functions.
Starting point for Assignment 4.

Build it and run tests by typing make.

These tests should work:

python3 testFSsmall.py | ./myShell
Is a test of the inode map functions.

./testParse tests the breakWords function that should be useful for
parsing directory inodes. The format is
name1|inode#
anotherName|inode#

Tests that won't work until the assignment is done:

python3 testFS.py | ./myShell

my_shell.c is a fairly bloated shell that has all of the previous
commands. I made a lighter-weight, simpler shell, new_shell.c, which
only supports the new functions. It gets turned into the newShell
executable.

Finally, there's a quite small test program of just the directory
cache functions, test_dir_cache.c. Its executable is testDirCache.
When it runs, it creates a small device, dctest.dev, which has two
top-level directories: test1 and test2. showTree is an executable that
just prints that device's directory tree. The source for both of these
should inform how the functions are supposed to work if the header and
stub implementations aren't clear enough.

The header is dir_cache.h. Stubs for the main functions are provided
in dir_cache.c. Since we're doing so many different things (python,
c++, c (mine and/or yours), these interfaces aren't set in stone.

For c/c++ your programs should run without any valgrind errors. No valgrind
leaks is extra credit.
