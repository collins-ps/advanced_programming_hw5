#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "graph.h"

void print_graph(Graph *g){
  Edgenode *e = NULL;
  for (int i=0;i<g->nvertices;++i){              /* for each node 0..NV-1 */
    e = g->edges[i];                             /* e=pointer to root of node i's edge list */
    while (e != NULL) {                          /* traverse list */
      printf("%d %d %d \n", i, e->y, e->weight); /* print edge and weight */
      e = e->next;
    }
  }
}

void read_graph(char *filename, Graph *g, bool directed){
  int nvertices, nedges;
  int u,v,w;
  FILE *file = NULL;
  file = fopen(filename,"r");
  fscanf(file,"%d %d", &nvertices, &nedges);     /* read header */
  //  printf("NV = %d    NE = %d\n",nvertices,nedges);
  

  g->nvertices = nvertices;                      /* initialize Graph fields */
  // printf("%d\n", nvertices);
  g->nedges = nedges;
  g->directed = true;
  g->edges = malloc((nvertices)*sizeof(Edgenode));
  
  for (int i=0;i<nvertices;++i)
    g->edges[i] = NULL;
  
  Edgenode *prev;
  while (fscanf(file, "%d %d %d\n", &u,&v,&w) != EOF) {
    Edgenode *new_node = (Edgenode *) malloc(sizeof(Edgenode));

    if (g->edges[u] == NULL) {       /* if first edge foor vertex u */
      g->edges[u] = new_node;
      g->edges[u]->y = v;
      g->edges[u]->weight = w;
      g->edges[u]->next = NULL;
      prev = new_node;
    } else {                         /* adding another edge to vertex u */
      prev->next = new_node;
      new_node->y = v;
      new_node->weight = w;
      new_node->next = NULL;
      prev = new_node;
    }
  }
  fclose(file);
}

void destroyGraph(Graph *g){
  for (int i=0; i<g->nvertices;++i){
    Edgenode *node = g->edges[i];
    while (node != NULL){
      Edgenode *next = node->next;
      free(node);
      node = next;
    }
  }
  free(g->edges);
}
