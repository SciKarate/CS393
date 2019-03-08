COMPILATION:
	To compile, just compile BitSet.cc with g++!
	Make sure that BitSet.h is in the same folder.

COMPILATION OPTIONS:
	Feel free to compile with -Wextra, -Wall, -Werror, and -pedantic. Nothing will change, though!

TESTING:
	BitSet.cc is a testing file.
	main() runs through some tests.
	The rest of the file aliases in helpful testing functions.

VALGRIND:
	Feel free to run valgrind!
	You'll be greeted with a lovely --
		"All heap blocks were fread -- no leaks are possible"
	You will, admittedly, also see a
		"Conditional jump or move depends on..."
	This occurs as a result of the line
		b->set[i] = int(ostr[i] - 48);
	In particular, the int(ostr[i] - 48) part.
	I don't know why this happens, but I've found the problem.

PROBLEMS:
	You need to manually free your BitSet* pointers when you're done using my implementation.
	There might be a way around this while still adhering to the described API, but I'm unaware of it.
	I know I could solve this problem by putting all of BitSet's methods into a class, but then I would be breaking the API
		e.g. SetBit(b,i) becomes b.SetBit(i)

IN-MEMORY METHODOLOGY:
	I used vector<bool> for my implementation.
	From everything I can find online (stackexchange, cppreference, cplusplus, etc), each entry takes 1 bit in memory.
	sizeof() disagrees, but as shown in my tests, when I write 512 bits to a BitSet, sizeof() says it is 40 bytes, or 320 bits. So sizeof() must be wrong.

IN-DISK METHODOLOGY:
	I encode each BitSet into decimal when writing, then back into binary when reading.
	I couldn't figure out how to get each bit to store as a bit in a text file, but I managed to decrease how much I was storing by encoding this way.