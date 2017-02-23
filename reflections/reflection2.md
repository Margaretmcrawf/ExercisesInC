My personal learning goals this sprint were to continue learning C and understand our project. I am a little behind where I would like to be on learning C, due to being one chapter behind on readings. By tomorrow, I will have done Chapter 5 of both ThinkOS and HFC, but won't have gotten to LBoS. This is something I plan on working on this weekend. I think I've done an okay job understanding the project, although actually documentable progress hasn't kept up with my understanding. I have been able to see how our program commands the arduino, and I have a conceptual understanding of how we will use clock cycles to precisely play different frequencies. 

My team is now down to two people from our original 4, so we have had to reevaluate some of our project goals. At our last sprint retro, we discussed which of our goals were realistic with our current manpower, and decided to focus on completing our MVP and possibly one of our stretch goals, which is harmonic frequencies. We have some good basics for how to make sine waves of a given frequency, but haven't quite gotten hardware timer interrupts to cooperate with us, so that's the current focus.

[HFC Exercise 3](../exercises/ex03) and [Exercise 4](../exercises/ex04)

[ThinkOS Reading Questions](../reading_questions) (Chapters 4 and 5 are new)

__Exam Question:__ Explain what is meant by the file abstraction, and what goals a file system should have when allocating storage.

__Answer:__ The file system abstraction is a relationship between a filename and a stream of bytes that represent that file. In reality, hard drives are divided into blocks, not bytes. 
A file system must keep track of which blocks are allocated to which files, make sure that blocks are being used to their full capacity, be fast, keep things contiguous when possible, and have minimal space overhead. 


