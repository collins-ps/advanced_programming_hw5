dj: dijkstra.c graph.c graph.h timer.c timer.h
	gcc -O3 -fopenmp -o dj dijkstra.c timer.c graph.c -I.

clean:
	\rm -f *.o dj *~ *#
