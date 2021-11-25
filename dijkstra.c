#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "graph.h"
#include "timer.h"
#include <omp.h>

#define M 10 // number of longest paths distances to output in datafile

typedef struct MinHeapNode_{
    int  v;
    int dist;
} MinHeapNode;
 
typedef struct MinHeap_{
    int size;    
    int capacity; 
    int *pos;   
    MinHeapNode **array;
} MinHeap;

typedef struct distPos_{
    int dist;
    int startVertex;
    int endVertex;
} distPos;

MinHeapNode* createMinHeapNode(int v, int dist);
MinHeap* createMinHeap(int capacity);
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b);
void Heapify(MinHeap* minHeap, int idx);
int isEmpty(MinHeap* minHeap);
MinHeapNode* extractMin(MinHeap* minHeap);
void decreaseKey(MinHeap* minHeap,int v, int dist);
bool isInMinHeap(MinHeap *minHeap, int v);
void destoyMinHeap(MinHeap *minHeap);
void dijkstra(Graph *g, int start, distPos *distance);
int distComparer(const void *a, const void *b);

int main(int argc, char **argv) {
    Graph g;
    char *filename;
    filename = argv[1];
    int nt;
    if (argc > 2)
        nt = atoi(argv[2]);
    else
        nt = 1; // default number of threads is 1
    read_graph(filename,&g,true);   
    distPos *topDistances = malloc(sizeof(distPos) * M * g.nvertices);
    double totalTime = 0;
    int position = 0;

    #pragma omp parallel for schedule(dynamic) num_threads(nt)
    for (int i = 0; i < g.nvertices; i++){
        distPos *dist = malloc(g.nvertices*sizeof(distPos));
        StartTimer(); 
        dijkstra(&g, i, dist);
        totalTime += GetTimer() / 1000.0;
        qsort(dist, g.nvertices, sizeof(distPos), distComparer);
        for (int j = 0; j < g.nvertices && j < M; j++){
            topDistances[position].dist = dist[j].dist;
            topDistances[position].startVertex = dist[j].startVertex;
            topDistances[position].endVertex = dist[j].endVertex;
            position++;
        }
        free(dist);
    }

    qsort(topDistances, position, sizeof(distPos), distComparer);

    FILE *datafile    = NULL; 
    datafile          = fopen("results.txt","a");
    fprintf(datafile, "File: %s Num threads: %d\n Start Vertex:    End Vertex:     Distance:\n", filename, nt);   
    printf("Start Vertex:    End Vertex:     Distance:\n");
    for (int i = 0; i < position && i < M; i++){
        printf("     %d            %d          %d\n",topDistances[i].startVertex, topDistances[i].endVertex, topDistances[i].dist);
        fprintf(datafile, "     %d            %d          %d\n",topDistances[i].startVertex, topDistances[i].endVertex, topDistances[i].dist);
    }
    printf("Time taken: %f (s) \n", totalTime);
    fprintf(datafile, "Time taken: %f (s) \n\n", totalTime);
    fclose(datafile);
    destroyGraph(&g);
    free(topDistances);
    return(0);
}

MinHeapNode* createMinHeapNode(int v, int dist){
    MinHeapNode* minHeapNode = (MinHeapNode*) malloc(sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}
 
MinHeap* createMinHeap(int capacity){
    MinHeap* minHeap = (MinHeap*) malloc(sizeof(MinHeap));
    minHeap->pos = (int*) malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**) malloc(capacity * sizeof(MinHeapNode*));
    return minHeap;
}
 
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b){
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
 
void Heapify(MinHeap* minHeap, int idx){
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
 
    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
      smallest = left;
 
    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
      smallest = right;
 
    if (smallest != idx){
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];
 
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
 
        swapMinHeapNode(&minHeap->array[smallest],&minHeap->array[idx]);
        Heapify(minHeap, smallest);
    }
}
 
int isEmpty(MinHeap* minHeap){
    return (minHeap->size == 0);
}
 
MinHeapNode* extractMin(MinHeap* minHeap){
    if (isEmpty(minHeap))
        return NULL;
    MinHeapNode* root = minHeap->array[0];
    MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
    --minHeap->size;
    Heapify(minHeap, 0);
    return root;
}
 
void decreaseKey(MinHeap* minHeap,int v, int dist){ // decrease dist of vertex v to dist
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist){
        minHeap->pos[minHeap->array[i]->v] =(i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}
 
bool isInMinHeap(MinHeap *minHeap, int v){
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

void destoyMinHeap(MinHeap *minHeap){
    free(minHeap->array);
    free(minHeap->pos);
    free(minHeap);
}

void dijkstra(Graph *g, int start, distPos *distance) {
    int i;
    Edgenode *p;
    bool *processed = malloc((g->nvertices)*sizeof(bool));
    int v, w, weight, dist;
    MinHeap* minHeap = createMinHeap(g->nvertices);
    for (i=0; i < g->nvertices; i++){
        processed[i]  = false;
        distance[i].dist = INT_MAX;
        distance[i].startVertex = start;
        distance[i].endVertex = i;
        minHeap->array[i] = createMinHeapNode(i,distance[i].dist);
        minHeap->pos[i] = i;
    }
    distance[start].dist = 0;
    v = start;                         /* v is current vertex being processed */
    decreaseKey(minHeap, v, 0);

    minHeap->size = g->nvertices;
    while (!isEmpty(minHeap)){
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;
        processed[u] = true;
        Edgenode* pEdge = g->edges[u];
        while (pEdge != NULL){
            int p = pEdge->y;
            if (processed[p] == false && distance[u].dist != INT_MAX && pEdge->weight + distance[u].dist < distance[p].dist){
                distance[p].dist = distance[u].dist + pEdge->weight;
                decreaseKey(minHeap, p, distance[p].dist);
            }
            pEdge = pEdge->next;
        }
        free(minHeapNode);
    }
    free(processed);
    destoyMinHeap(minHeap);
    for (i=0; i < g->nvertices; i++){
        if (distance[i].dist == INT_MAX)
            distance[i].dist = -1;
    }
    return;
}

int distComparer(const void *a, const void *b){ 
  distPos *pa = (distPos *) a;
  distPos *pb = (distPos *) b;

  if ( pa->dist < pb->dist)
    return 1;
  else if (pa->dist == pb->dist) 
    return 0;
  else 
    return -1;
}
