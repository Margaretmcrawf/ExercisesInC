## Chapter 1


### Compilation

1) Give an example of a feature common in interpreted languages that is rare in compiled languages.

Memory allocation can be more efficient because you allocate things just in time, also running them is 
platform independent. Debugging is different as well, because code that doesn't run and code that runs but 
doesn't behave as expected are seen in the same step.

2) Name two advantages of static typing over dynamic typing.

Your language has built in protections against doing things you shouldn't, which might end up with your code
compiling/running but not doing what it should. It can also be faster because the memory is allocated 
beforehand.

3) Give an example of a static semantic error.

Trying to add to a variable before giving it a value.

4) What are two reasons you might want to turn off code optimization?

Trying to debug assembly code and wanting it readable, you want to do something a little bit sketchy that you
think the compiler will try to optimize out. 

5) When you run `gcc` with `-S`, why might the results look different on different computers?

Because assembly is a way to talk to the machine, so different machine architecture requires different assembly.

6) If you spell a variable name wrong, or if you spell a function name wrong, 
the error messages you get might look very different.  Why?

Because they are caught in two different steps. Variables fail during compile time (generating assembly, defining 
memory locations), functions fail during link time (figuring out how the pieces fit together).

7) What is a segmentation fault?

It occurs when a program tries to read or write to the wrong kind of memory, for example writing to read-only memory.


## Chapter 2


### Processes

1) Give a real-world example of virtualization (ideally not one of the ones in the book).

Routers aren't actually connected to that much, but since those other routers are connected to other routers it gives 
the appearance that every machine is connected to every other one through the internet.

2) What is the difference between a program and a process?

A program is a piece of code that can be run, a process is a running program and all the associated data.

3) What is the primary purpose of the process abstraction?  What illusion does the process abstraction create?

The illusion is that the process has uninterrupted CPU time and unlimited, continuous memory space, neither of which are
true. The goal is so that the user doesn't have to worry about how different processes interact with eachother.

4) What is the kernel?

The core of a computer's operating system. It mediates access to the computer's resources, which includes deciding who gets
access to the processors and RAM, and dealing with requests to input/output devices. 

5) What is a daemon?

A friendly process that runs in the background.
 

## Chapter 3


### Virtual memory

1) The Georgian alphabet has 33 letters.  How many bit are needed to specify a letter?

6, because it's less that 64 and more than 32, so 2^6 options are required.

2) In the UTF-16 character encoding, the binary representation of a character can take up to 32 bits.  
Ignoring the details of the encoding scheme, how many different characters can be represented?

2^32, or about 4 billion characters.

3) What is the difference between "memory" and "storage" as defined in Think OS?

Memory is volatile, storage is not.

4) What is the difference between a GiB and a GB?  What is the percentage difference in their sizes?

A Gib is a binary unit (2^30 bytes), while a GB is a base 10 unit (10^9 bytes). They are fairly similar in size,
but a Gib is about 7% larger.

5) How does the virtual memory system help isolate processes from each other?

Because a process can't generate the virtual address that maps to the physical address another process has used.
It's done on a pre-process basis, so the processes don't run into eachother. 

6) Why do you think the stack and the heap are usually located at opposite ends of the address space?

So that you don't have to have an idea of how big the stack will be in order to start the heap. They only run into
eachother in the middle, or when all of the space is taken up anyway. 

7) What Python data structure would you use to represent a sparse array?

I think there's a scipy object that does this...

8) What is a context switch?

When an OS stops a running process, saves its state, and then runs another process.

In this directory, you should find a subdirectory named `aspace` that contains `aspace.c`.  Run it on your computer and compare your results to mine.

My results:

Address of main is 0x4005d6
Address of global is 0x60104c
Address of local is 0x7ffd2d9bec4c
Address of p is 0x989010

Mine are relatively similar, p is a little lower (my heap was able to give it a lower value) but they're generally similar.
  
1) Add a second call to `malloc` and check whether the heap on your system grows up (toward larger addresses).  

Address of p is 0x1362010
Address of q is 0x13620a0

The second call to malloc has increased by 0x10 (1 -> a in the 10's place)

2) Add a function that prints the address of a local variable, and check whether the stack grows down.  

Address of local is   0x7fff429c2090
Address of local_2 is 0x7fff429c2094

It's actually slightly higher. I don't know why that is.

3) Choose a random number between 1 and 32, and allocate two chunks with that size.  
How much space is there between them?  Hint: Google knows how to subtract hexadecimal numbers.


## Chapter 4


### Files and file systems

1) What abstractions do file systems provide?  Give an example of something that is logically 
true about files systems but not true of their implementations.

The file system is a mapping between a file name and its contents, which are bytes. In reality, storage is block-based.

2) What information do you imagine is stored in an `OpenFileTableEntry`?

What information has been given to the program from the file, what information has been requested, how many times it has been requested.

3) What are some of the ways operating systems deal with the relatively slow performance of persistent storage?

When they get a call, they might get more than they needed to so that if a program asks for the next block as well it can get that easily, they keep things that have been called recently in a cache, and they use buffering, where they don't write to storage immediately, they keep it in memory for a while.

4) Suppose your program writes a file and prints a message indicating that it is done writing.  
Then a power cut crashes your computer.  After you restore power and reboot the computer, you find that the 
file you wrote is not there.  What happened?

It was still in the buffer when the computer crashed, so even though it said it was done writing it was still in memory, not storage.

5) Can you think of one advantage of a File Allocation Table over a UNIX inode?  Or an advantage of a inode over a FAT?

An inode doesn't work very well for very large files. However, you might want to use one in situations where you need a lot of speed, because inode is better for that.

6) What is overhead?  What is fragmentation?

Overhead is the parts of the storage that aren't the file itself, ie the data structures which probably include some pointers to other blocks, etc. 
Fragmentation is unused space in blocks, due to them being unused or partially used.

7) Why is the "everything is a file" principle a good idea?  Why might it be a bad idea?

It works pretty well because bytes make more sense to people than blocks do.

If you would like to learn more about file systems, a good next step is to learn about journaling file systems.  
Start with [this Wikipedia article](https://en.wikipedia.org/wiki/Journaling_file_system), then 
[Anatomy of Linux Journaling File Systems](http://www.ibm.com/developerworks/library/l-journaling-filesystems/index.html).  
Also consider reading [this USENIX paper](https://www.usenix.org/legacy/event/usenix05/tech/general/full_papers/prabhakaran/prabhakaran.pdf).


## Chapter 5


### Bits and bytes

1) Suppose you have the value 128 stored as an unsigned 8-bit number.  What happens if you convert 
it to a 16-bit number and accidentally apply sign extension?

You start with 1000 0000, then if you accidentally use sign extension you get 1111 1111 1000 0000, which might be evaluated as -127.

2) Write a C expression that computes the two's complement of 12 using the XOR bitwise operator. 
Try it out and confirm that the result is interpreted as -12.

(12 ^ 15) + 1

3) Can you guess why IEEE floating-point uses biased integers to represent the exponent rather than a
sign bit or two's complement?

Probably because an overflow error would be really annoying in that case, so it's safer to just have a bias.

4) Following the example in Section 5.4, write the 32-bit binary representation of -13 in single precision 
IEEE floating-point.  What would you get if you accidentally interpreted this value as an integer?

1  -> 1

130 -> 1000 0010 

0x500000 -> 101 0000 0000 0000 0000 0000

1100 0001 0101 0000 0000 0000 0000 0000

If this was read as a signed integer it would be -1095761920

5) Write a function that takes a string and converts from lower-case to upper-case by flipping the sixth bit.  
As a challenge, you can make a faster version by reading the string 32 or 64 bits at a time, rather than one
character at a time.  This optimization is made easier if the length of the string is a multiple of 4 or 8 bytes.

```c

char[] toUppercase(char *string, int len) {
	for (int i = 0; i < len; i++) {
		char character = string[i];
		string[i] = character ^ 32;
}
	return string;
}

```



## Chapter 6


### Memory management

1) Which memory management functions would you expect to take constant time?  Which ones take time proportional to the size of the allocated chunk?

2) For each of the following memory errors, give an example of something that might go wrong:

a) Reading from unallocated memory.

b) Writing to unallocated memory.

c) Reading from a freed chunk.

d) Writing to a freed chunk.

e) Failing to free a chunk that is no longer needed.


3) Run

    ps aux --sort rss

to see a list of processes sorted by RSS, which is "resident set size", the amount of physical 
memory a process has.  Which processes are using the most memory?

4) What's wrong with allocating a large number of small chunks?  What can you do to mitigate the problem?

If you want to know more about how malloc works, read 
[Doug Lea's paper about dlmalloc](http://gee.cs.oswego.edu/dl/html/malloc.html)



## Chapter 7


### Caching

1) What happens if a program writes a new value into the program counter?

2) What is the fundamental problem caches are meant to solve?

3) If cache access time is 1 ns and memory access time is 10 ns, what is the average
access time of a program with hit rate 50%?  How about 90%?

4) The book gives several examples of programming language features, like loops, that tend 
to improve locality in the access pattern of instructions and/or data.  Can you think of other examples?  
Or counter-examples that might decrease locality?

5)  If you refactor a program to improve locality, would you say the program is "cache aware"?  Why not?

6) See if you can estimate the cost of a memory cache by comparing the prices of two similar CPUs with 
different cache sizes.

7) Why are cache policies generally more complex at the bottom of the memory hierarchy?

8) Can you think of a strategy operating systems could use to avoid thrashing or recover when it occurs?

Run the cache code on your laptop or another computer and see if you can infer the cache size and block size.  
If you can find the technical specifications for your computer, see if your inferences are right.

1) In this directory you should find a subdirectory named `cache` that contains `cache.c` and supporting files.  Read `cache.c`, then run `make` and `./cache`.

2) Run `python graph_data.py` to see the results.  What can you infer about the cache structure on your computer?


## Chapter 8


### Multitasking

1) What is the kernel's most basic task?

2) When an interrupt occurs, what part of the hardware state is saved by hardware?

3) What is the difference between an interrupt and a context switch?

4) Give an example of an event that might cause a process to move from the blocked to the ready state.

5) Why might a scheduler want to give higher priority to an I/O bound process?

When I make French toast, I usually make a batch of 12 slices.  But my griddle only has room for 8 slices.  Each piece of toast has to cook for 5 minutes on each side.  How can I schedule 12 slices onto 8 "cores" to minimize the elapsed time to cook all 12 slices?  (Note: this question is not hypothetical; this is really how I make French toast.)



## Chapter 9


### POSIX threads and mutexes

As you read Chapter 9, you should compile and run the example code.  By the nature of multi-threaded programs, you results will probably be different from mine, possibly in interesting ways.

1) Why does each thread have its own stack?

2) What does the `gcc flag -lpthread` do?

3) What does the argument of `pthread_exit` do?

4) Normally the same thread that created a thread also waits to join it.  What happens if another thread tries to join a thread it did not create?

5) What goes wrong if several threads try to increment a shared integer at the same time?

6) What does it mean to "lock a mutex"?



## Chapter 10


### Condition variables

1) What does it mean to say that a data structure is thread safe?

2) In the circular buffer implementation of a queue, why is the maximum number of elements in the queue `n-1`,
if `n` is the size of the array?

3) If there is no mutex to protect the queue, give an example of a sequence of steps that could leave
the queue in an inconsistent state.

4) When a thread calls cond_wait, why does it have to unlock the mutex before blocking?

5) When a thread returns from cond_wait, what do we know is definitely true?  What do we think is probably true?

6) What happens if you signal a condition variable when there are no waiting threads?

7) Do you have to lock the mutex to signal a condition variable?

8) Does the condition have to be true when you signal a condition variable?


 
## Chapter 11


### Semaphores in C
