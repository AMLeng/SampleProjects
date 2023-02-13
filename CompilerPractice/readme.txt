Originally, this was going to be an implementation of a compiler for the variant of the "Decaf" programming language as specified in the Summer 2012 version of Stanford's CS143 Compilers Class.

Resources for the class are available at https://web.stanford.edu/class/archive/cs/cs143/cs143.1128/

Note that as "Decaf" is a commonly used name for a variety of different simple "Java-like" programming languaged used in introductory compilers courses, and that different incarnations use totally different specifications. I've included a copy of the specification in this repository. Furthermore, the specification is slightly ambiguous, so I've listed the resolutions to ambiguities at the bottom.

However, due to some suboptimal design decisions which would make finishing this project relatively suboptimal as a learning experience, I've decided to leave this in an incomplete state, with a working decaf lexer, and working (general purpose!) parser generator, but without AST generation, analysis, or code generation. 
