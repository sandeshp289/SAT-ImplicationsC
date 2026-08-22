# SAT-ImplicationsC

Description:

    Determination of SAT based logic implications in C/C++

Input Files:

    SAT_implications.c             : C/C++ code
  
    c17.cnf                        : Input CNF file (for e.g. combinational circuit c17)
    
Compilation:

    g++ SAT_implications.c
    
Execution:

    ./a.out c17 <assignment1> <assignment2> ...

Output for ./a.out c17 -7

	Successfully read in the CNF file:  
	13 total number of variables.  
	22 total number of clauses.
	The implications are: 
  
	---------------------- 

	-13  	-11  	9  	8  	4  	3 


Output for ./a.out c17 -1

	Successfully read in the CNF file:
	13 total number of variables.
	22 total number of clauses.

	The implications are: 
	---------------------- 

	6  

Output for ./a.out c17 1 3

	Successfully read in the CNF file:
	13 total number of variables.
	22 total number of clauses.

	The implications are: 
	---------------------- 

	12  	10  	-6  
    

    

