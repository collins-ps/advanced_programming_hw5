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
I did not observe any compilation or run-time errors. The Valgrind report is clean. Please use the edited version of graph1.txt (comments removed) and updated versions of graph.h and graph.c uploaded to the main branch.

### Results

**Graph 1**:

Start Vertex,    End Vertex,     Distance<br>
     644,            6278,          192<br>
     644,            6290,          186<br>
     5720,           6278,          186<br>
     645,            6278,          184<br>
     3634,           6278,          184<br>
     644,            6187,          183<br>
     4568,           6278,          183<br>
     644,            6188,          181<br>
     4193,           6278,          181<br>
     5995,           6278,          181<br>
   
   
**Graph 2**:

Start Vertex,    End Vertex,     Distance<br>
    100201,          36397,          103<br>
    100201,          150246,         103<br>
    41354,           36397,          102<br>
    41354,           150246,         102<br>
    100201,          92762,          102<br>
    100201,          139117,         102<br>
    26333,           36397,          101<br>
    26333,           150246,         101<br>
    40743,           36397,          101<br>
    40743,           150246,         101<br>

### Performance
Program was compiled using -g -O3 and run on the Linux server from out of state.

**Graph 1**:

          nThreads         totalTime (s)          Speedup
             64              0.434174               9.9 
             16              0.649108               6.6 
             8               0.997707               4.3 
             1               4.293279                NA 
              
              
**Graph 2**:

          nThreads         totalTime (s)          Speedup
             64             794.618847               12                
             16             1706.418450              5.6
             1              9543.258867              NA 
             
## Compiling and running
`make dj` and then `./dj filename num_threads` (e.g.`./dj graph1.txt 16`). If there is no input for num_threads, program will run serially with 1 thread. Finally, run `make clean`.
