# RPAL-Parser-and-CSE-Machine
The objective of Project 1 is to implement a lexical analyzer, and a recursive descent parser for the RPAL language. The objective of Project 2 is to implement the sub-tree (partial) standardizer, and the 13-rule CSE machine.


First use the "make" command to compile the project.
Then place the source code and header files and difftest.pl and the tests folder inside a single folder.
To this parent folder add the RPAL interpreter also.

Then use the below command to run the code for all the cases within the tests folder.
The below command executes the project for these test cases. Also, it runs RPAL on the same test cases and as an output generates a folder named "results" within the parent folder. 

The "results" folder shall contain the test cases that did not match the RPAL output. It will also have specific details pointing out what exactly was the mismatch.

Command:
--------

perl difftest.pl -1 "./rpal FILE" -2 "./p2 FILE" -t ./tests/ -r results -v 2
