# Operating System Assignment

# Banker-s-Algorithm

# Question

Write a multithreaded program that implements the banker's algorithm.Create n threads that request and release resources from the bank.
The banker will grant the request only if it leaves the system in a safe state.It is important that shared data be safe from concurrent access.
To ensure safe access to shared data, you can use mutex locks.


# What is Banker's Algorithm?

=>Banker's algorithm is a deadlock avoidance algorithm.
  It is named so because this algorithm is used in banking systems to determine whether a loan can be granted or not.
  Consider there are n account holders in a bank and the sum of the money in all of their accounts is S.
  Everytime a loan has to be granted by the bank, it subtracts the loan amount from the total money the bank has. 
  Then it checks if that difference is greater than S.
  It is done because, only then, the bank would have enough money even if all the n account holders draw all their money at once.

=>Banker's algorithm works in a similar way in computers.
  Whenever a new process is created, it must specify the maximum instances of each resource type that it needs, exactly.

Let us assume that there are n processes and m resource types. 
Some data structures that are used to implement the banker's algorithm are:

1. Available
It is an array of length m. It represents the number of available resources of each type. If Available[j] = k, then there are k instances available, of resource type R(j).

2. Max
It is an n x m matrix which represents the maximum number of instances of each resource that a process can request. If Max[i][j] = k, then the process P(i) can request atmost k instances of resource type R(j).

3. Allocation
It is an n x m matrix which represents the number of resources of each type currently allocated to each process. If Allocation[i][j] = k, then process P(i) is currently allocated k instances of resource type R(j).

4. Need
It is an n x m matrix which indicates the remaining resource needs of each process. If Need[i][j] = k, then process P(i) may need k more instances of resource type R(j) to complete its task.

Need[i][j] = Max[i][j] - Allocation [i][j]



# Solution

Solution :- [For code, Refer to Banker's Algorithm.c in the repository]

# Compile
gcc bankers_algorithm.c -std=c99 -pthread -o bankers_algorithm


# Input

Number of processes? 5
Number of resources? 3

Currently Available resources (R1 R2 ...)? 3 3 2

Resource allocated to process 1 (R1 R2 ...)? 0 1 0
Resource allocated to process 2 (R1 R2 ...)? 2 0 0
Resource allocated to process 3 (R1 R2 ...)? 3 0 2
Resource allocated to process 4 (R1 R2 ...)? 2 1 1
Resource allocated to process 5 (R1 R2 ...)? 0 0 2

Maximum resource required by process 1 (R1 R2 ...)? 7 5 3
Maximum resource required by process 2 (R1 R2 ...)? 3 2 2
Maximum resource required by process 3 (R1 R2 ...)? 9 0 2
Maximum resource required by process 4 (R1 R2 ...)? 2 2 2
Maximum resource required by process 5 (R1 R2 ...)? 4 3 3


# Output

Safe Sequence Found : 2  4  5  1  3
Executing Processes...

--> Process 2
	Allocated :   2  0  0
	Needed    :   1  2  2
	Available :   3  3  2
	Resource Allocated!
	Process Code Running...
	Process Code Completed...
	Process Releasing Resource...
	Resource Released!
	Now Available :   5  3  2


--> Process 4
	Allocated :   2  1  1
	Needed    :   0  1  1
	Available :   5  3  2
	Resource Allocated!
	Process Code Running...
	Process Code Completed...
	Process Releasing Resource...
	Resource Released!
	Now Available :   7  4  3


--> Process 5
	Allocated :   0  0  2
	Needed    :   4  3  1
	Available :   7  4  3
	Resource Allocated!
	Process Code Running...
	Process Code Completed...
	Process Releasing Resource...
	Resource Released!
	Now Available :   7  4  5


--> Process 1
	Allocated :   0  1  0
	Needed    :   7  4  3
	Available :   7  4  5
	Resource Allocated!
	Process Code Running...
	Process Code Completed...
	Process Releasing Resource...
	Resource Released!
	Now Available :   7  5  5


--> Process 3
	Allocated :   3  0  2
	Needed    :   6  0  0
	Available :   7  5  5
	Resource Allocated!
	Process Code Running...
	Process Code Completed...
	Process Releasing Resource...
	Resource Released!
	Now Available :  10  5  7


All Processes Finished
