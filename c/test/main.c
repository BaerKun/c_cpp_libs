#include <stdio.h>
#include "adjacency_list/Euler_path.h"
#include "adjacency_list/find_scc.h"

int main(){
    Graph graph;
    graphInit(&graph, 10);

    for(int i = 0; i < 5; i++){
        graphAddVertex(&graph, (VertexData){0});
    }

    graphAddEdge(&graph, 0, 1, (EdgeData){0}, 1);
    graphAddEdge(&graph, 1, 2, (EdgeData){0}, 1);
    graphAddEdge(&graph, 2, 3, (EdgeData){0}, 1);
    graphAddEdge(&graph, 3, 4, (EdgeData){0}, 1);
    graphAddEdge(&graph, 4, 0, (EdgeData){0}, 1);

//    int number[5];
//    graphFindScc(&graph, number);

//    for(int i = 0; i < 5; i++) {
//        printf("%d ", number[i]);
//        number[i] = -1;
//    }
//
//    graphFindScc(&graph, number);
//
//    for(int i = 0; i < 5; i++) {
//        printf("%d ", number[i]);
//        number[i] = -1;
//    }

    LinkNode path;
    EulerCircuit(&graph, &path, 0);

    for(LinkNode *p = &path; p; p = p->next){
        printf("%d ", p->data);
    }
    nodeClear(&path.next);

    putchar('\n');
    graphAddEdge(&graph, 0, 2, (EdgeData){0}, 1);
    EulerPath(&graph, &path, 0, 2);
    for(LinkNode *p = &path; p; p = p->next) {
        printf("%d ", p->data);
    }
    graphDestroy(&graph);
}