# Homework 5

Write a C program that solves the all source shortest path problem by applying Dijkstra n = |V| times –– that is, once for each vertex in the input graph G, as described in the recorded lectures. An important detail not described in typical Dijkstra pseudocode is the implementation of the next-closest vertex search. To get full credit, and to ensure the algorithm runs in reasonable time for the large input file provided, the search must be done using a binary min-heap priority queue*. To speed up execution of the large graph, you must also parallelize your code using OpenMP. Rather than save all of the path data, output only the 10 longest paths in a file with format:

start vertex 1, end vertex 1, distance 1<br>
start vertex 2, end vertex 2, distance 2<br>
...<br>
...<br>
start vertex 10, end vertex 10, distance 10

In addition to devising and running your own correctness tests, carry out a performance analysis on the two graphs provided. No credit will be given for extremely inefficient implementations (several hours is the expected execution time in serial for large graph).

*An excellent reference for building a min-heap priority queue can be found here: https://bradfieldcs.com/algos/trees/priority-queues-with-binary-heaps/

# MPCS 51100 HW5
**Name**: Phoebe Collins, UCID: 12277438

## References
I referenced class materials and geekforgeeks (https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/).

## Discussion on program and performance
I did not observe any compilation or run-time errors. The Valgrind report is clean.

### Results
**Graph 1**:
Start Vertex:    End Vertex:     Distance:
     644            6278          192
     644            6290          186
     5720           6278          186
     645            6278          184
     3634           6278          184
     644            6187          183
     4568           6278          183
     644            6188          181
     4193           6278          181
     5995           6278          181
   
**Graph 2**:
Start Vertex:    End Vertex:     Distance:
    100201          36397          103
    100201          150246         103
    41354           36397          102
    41354           150246         102
    100201          92762          102
    100201          139117         102
    26333           36397          101
    26333           150246         101
    40743           36397          101
    40743           150246         101

### Performance
Program was compiled using -o3 and run on the Linux server from out of state.
**Graph 1**:
          nThreads         totalTime (s)          Speedup
             64              0.501007               16.6 
             16              1.022393               8.12 
             8               1.841065               4.5 
             1               8.302425                NA 
              
**Graph 2**:
          nThreads         totalTime (s)          Speedup
             64              0.501007               16.6 
             16              1.022393               8.12 
             8               1.841065               4.5 
             1               8.302425                NA 
             
## Compiling and running
`make dj` and then `./dj filename num_threads` (e.g.`./dj graph1.txt 16`). If there is no input for num_threads, program will run serially with 1 thread. Finally, run `make clean`.
