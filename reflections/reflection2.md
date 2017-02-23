My personal learning goals were to continue learning C and improve on our project. I don't think I've done the best job with either of those. My team is now down to two people from our original 4, so we have had to reevaluate some of our project goals. At our last sprint retro, we discussed which of our goals were realistic with our current manpower, and decided to focus on completing our MVP and possibly one of our stretch goals, which is harmonic frequencies. Both my teammate and I are currently one chapter behind where we would like to be at this point, we have done ThinkOS 5 and are working on HFC 5 by Thursday, and haven't done any of LBoS. I will be trying to catch up this weekend, but for now we know that we are a bit behind and are trying our best. On the project, we have some good basics for how to make sine waves of a given frequency, but haven't quite gotten hardware timer interrupts to cooperate with us, so that's the current focus.

[HFC Exercise 3](../exercises/ex03) and [Exercise 4](../exercises/ex04)

[ThinkOS Reading Questions](../reading_questions) (Chapters 4 and 5 are new)

__Exam Question:__ Explain what is meant by the file abstraction, and what goals a file system should have when allocating storage.

__Answer:__ The file system abstraction is a relationship between a filename and a stream of bytes that represent that file. In reality, hard drives are divided into blocks, not bytes. 
A file system must keep track of which blocks are allocated to which files, make sure that blocks are being used to their full capacity, be fast, keep things contiguous when possible, and have minimal space overhead. 


