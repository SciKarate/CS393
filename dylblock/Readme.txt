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

/////////////////////
Hi!
I've added 'cd ..' functionality in my_shell.c
I've also done the actual assignment. It is in directory_cache.c
There are also some testing .py files that can be piped to ./myShell

//tester1.py | ./myShell
tester1 does the following:
	1) make two directories
	2) cd into one of them, make a new directory, then unmount.
	2*) before and after step 2, 'cd .' to see if anything breaks
	3) mount, cd into the other, make two directories, then unmount
	4) mount, check that the first directory has accurate contents
	5) mount, check that the second directory has accurate contents
	6) cd .. to show off, then try to cd .. in root.

tester2.py | ./myShell
tester2 does the following:
	1) make 5 directories
	2) loop 9 directories deep, making them along the way.
	2*) at 6th directory, create a sibling.
	3) cd .. alllllllll the way back, never ls'ing until the very top.
	4) unmount
	5) mount, cd all the way down, ls'ing to check for accurate contents.

///SHAME ZONE///THE ZONE OF SHAME///
a) I wrote a function in filesystem.c that crawls around the whole directory cache freeing directory entries to avoid memory leakage.
This does not avoid leakage, and somehow just turns ~1/2 of my "indirectly lost" bytes into "directly lost" bytes. It never segfaults, so I don't think I'm freeing things I shouldn't.

b) I can't figure out where my frees and allocs are all happening. ./testDirCache claims I make 30 allocs and 14 frees, but I don't think those are all happening in directory_cache.c.
./testDirCache makes a directory, reads it, then makes a new one. Each directory should take 3 allocs to read from file and 1 alloc to create from mkdir. Therefore, we should see
1 + 3 + 1 = 5 allocs for ./testDirCache
This is reinforced by the fact that my broken function in a) increases the number of frees by 5.
However, 30 != 5. So I am confused.

c) ./testDirCache does not throw any errors in valgrind (hooray!!)
However, all of my tests using the shell do (as many as 61!!).
It is tempting to say "I did not write the shell so that is not my fault." However, some of the errors look like they're coming from directory_cache.c. This is very confusing, and I haven't been able to diagnose the problem. I have tried a few things that I thought might work, but to no avail.

d) I didn't do the extra credit, and I'm sad about that, because it looked fun.
///SHAME CONCLUDED///