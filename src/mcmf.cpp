//
// Created by didi on 2019/7/2.
//

#include "../include/mcmf.h"

const int GLP_A_CAP = offsetof(ArcData, cap);
const int GLP_A_COST = offsetof(ArcData, cost);
const int GLP_A_LOW = offsetof(ArcData, low);
const int GLP_A_RC = offsetof(ArcData, rc);
const int GLP_A_X = offsetof(ArcData, x);
const int GLP_A_SIZE = sizeof(ArcData);
const int GLP_V_RHS = offsetof(VertexData, rhs);
const int GLP_V_SIZE = sizeof(VertexData);


glp_graph *createGraph() {
    return glp_create_graph(GLP_V_SIZE, GLP_A_SIZE);
};

glp_prob *createProblem() {
    return glp_create_prob();
}

void addVertices(glp_graph *G, int verticesNum) {
    int ret = glp_add_vertices(G, verticesNum);
    for (int i = 1; i <= verticesNum; ++i) {
        char name[5];
        sprintf(name, "v%d", i);
        printf("vertex: %s\n", name);
        glp_set_vertex_name(G, i, name);
    }
};

void setVertexRhs(glp_graph *G, int idx, double rhs) {
    VertexData *vd = (VertexData *) (G->v[idx]->data);
    vd->rhs = rhs;
};

void addArc(glp_graph *G, int srcIdx, int dstIdx, double low, double cap, double cost) {
    glp_arc *arc = glp_add_arc(G, srcIdx, dstIdx);
    ArcData *ad = (ArcData *) (arc->data);
    ad->low = low;
    ad->cap = cap;
    ad->cost = cost;
};

/**
 * 将最大流转为线性问题求解. 从1开始
 */
void maxFlowGraph2LinearProblem(glp_graph *G, glp_prob *P, const int srcIdx, const int sinkIdx) {
    if (G == NULL) {
        return;
    }

    if (P == NULL) {
        P = glp_create_prob();
    }
    glp_maxflow_lp(P, G, GLP_ON, srcIdx, sinkIdx, GLP_A_CAP);


    glp_write_lp(P, NULL, "maxFlow.lp");
    glp_write_maxflow(G, srcIdx, sinkIdx, GLP_A_COST, "maxflow.dimacs");

    return;
};

/**
 * 将最小费用流转发为线性问题求解
 */
void minCostGraph2LinearProblem(glp_graph *G, glp_prob *P) {
    if (G == NULL) {
        return;
    }
    if (P == NULL) {
        P = glp_create_prob();
    }

    glp_mincost_lp(P, G, GLP_ON, GLP_V_RHS, GLP_A_LOW, GLP_A_CAP, GLP_A_COST);

    //test
    glp_write_lp(P, NULL, "minCost.lp");
    glp_write_mincost(G, GLP_V_RHS, GLP_A_LOW, GLP_A_CAP, GLP_A_COST, "mincost.dimacs");

    return;
};

/**
    求解LP
*/
bool solveLP(glp_prob *P, double *optimalValue) {
    glp_smcp parm;

    glp_init_smcp(&parm);
    parm.msg_lev = GLP_MSG_OFF;
    parm.presolve = GLP_ON;

    int ret = glp_simplex(P, &parm);
    if (ret == 0) {
        *optimalValue = glp_get_obj_val(P);
        printf("optimalValue:%2.f\n", *optimalValue);
    }
    return ret == 0;
}

/**
 * 求解MCMF
 * @param srcIdx
 * @param sinkIdx
 */
bool solve(glp_graph *G, glp_prob *P, const int srcIdx, const int sinkIdx) {
    bool ok = false;

    double optimalValue = 9999999;

    maxFlowGraph2LinearProblem(G, P, srcIdx, sinkIdx);

    bool solveStatus = solveLP(P, &optimalValue);

    glp_print_sol(P, "mf.sol");

    if (solveStatus) {
        ok = false;
        double maxFlow = optimalValue;
        printf("MAX FLOW||OV=%4.f\n", optimalValue);

        setVertexRhs(G, srcIdx, maxFlow);
        setVertexRhs(G, sinkIdx, -maxFlow);

        minCostGraph2LinearProblem(G, P);
        solveStatus = solveLP(P, &optimalValue);

        if (solveStatus) {
            ok = true;
            //arcs = getResultArcs();
            //printf("MCMF||succeed\n");
            glp_print_sol(P, "mcmf.sol");

        } else {
            printf("MCMF||max flow succeed||min cost fail\n");
        }

    } else {
        printf("MCMF||max flow fail\n");
    }

    return ok;
}



void freeGraph(glp_graph *G) {
    if (G != NULL) {
        glp_delete_graph(G);
        G = NULL;
    }

}

void freeProblem(glp_prob *P) {
    if (P != NULL) {
        glp_delete_prob(P);
        P = NULL;
    }
}

