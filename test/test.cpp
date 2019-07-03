#include <iostream>

#include "../include/glpk.h"
#include "../include/mcmf.h"

using namespace std;

//void mcmf_test() {
//    MCMF mcmf;
//    mcmf.addVertices(8);
//
//    ////    vdata = glp_python_vertex_data_get(graph, 1);
////    vdata->rhs = 45;
////    vdata = glp_python_vertex_data_get(graph, 8);
////    vdata->rhs = -45;
////
////
//    int startNodes[] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 5, 6};
//    int endNodes[] = {1, 2, 3, 4, 5, 6, 4, 5, 6, 4, 5, 6, 7, 7, 7};
//    int capacities[] = {15, 15, 15, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 10, 20, 15};
//    int unitCosts[] = {0, 0, 0, 9, 12, 10, 8, 15, 12, 13, 17, 19, 0, 0, 0};
////
////    mcmf.setVertexRhs(1, 45);
////    mcmf.setVertexRhs(8, -45);
//
//    for (int i = 0; i < 15; ++i) {
//        mcmf.addArc(startNodes[i] + 1, endNodes[i] + 1, 0., (double) capacities[i], unitCosts[i]);
//    }
//
//    vector<Arc> ret = mcmf.solve(1, 8);
//    printf("optimal value: %4.f\n", mcmf.getOptimalValue());
//    for (auto arc : ret) {
//        printf("arc: src=%d, dst=%d, flow=%4.f\n", arc.src, arc.dst, arc.flow);
//    }
//
//}


void mcmf_test() {

    int startNodes[] = {0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 5, 6};
    int endNodes[] = {1, 2, 3, 4, 5, 6, 4, 5, 6, 4, 5, 6, 7, 7, 7};
    int capacities[] = {15, 15, 15, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 10, 20, 15};
    int unitCosts[] = {0, 0, 0, 9, 12, 10, 8, 15, 12, 13, 17, 19, 0, 0, 0};


    glp_graph * G = createGraph();
    glp_prob * P = createProblem();
    addVertices(G, 8);
    setVertexRhs(G, 1, 45);
    setVertexRhs(G, 8, 45);


    for (int i = 0; i < 15; ++i) {
        addArc(G, startNodes[i] + 1, endNodes[i] + 1, 0., (double) capacities[i], unitCosts[i]);
    }

    solve(G, P, 1, 8);
//    vector<Arc> ret = mcmf.solve(1, 8);
//    printf("optimal value: %4.f\n", mcmf.getOptimalValue());
//    for (auto arc : ret) {
//        printf("arc: src=%d, dst=%d, flow=%4.f\n", arc.src, arc.dst, arc.flow);
//    }

}


int main() {
    std::cout << "Hello, World!" << std::endl;
    //glpk_test();
    //graph_test();
    mcmf_test();
    return 0;
}
