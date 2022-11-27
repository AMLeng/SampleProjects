We implement a compiler for the variant of the "Decaf" programming language as specified in the Summer 2012 version of Stanford's CS143 Compilers Class.

Resources for the class are available at https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/

Note that as "Decaf" is a commonly used name for a variety of different simple "Java-like" programming languaged used in introductory compilers courses, and that different incarnations use totally different specifications. I've included a copy of the specification in this repository. Furthermore, the specification is slightly ambiguous, so I've listed the resolutions to ambiguities at the bottom.

Finally, while there were originally test programs provided for students of the course, I have been unable to find them and thus have written my own programs.


Resolutions to Ambiguities and Implementation Decisions:
I implemented the "Print" function (which originally should have been part of some provided library, but which I don't have access to) as:
    --Can take any number of arguments
    --Prints the arguments, separated by spaces, and then prints a newline at the very end (note that the our language spec prohibits strings from containing newline characters)
