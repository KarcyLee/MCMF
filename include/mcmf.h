//
// Created by didi on 2019/7/2.
//

#ifndef MCMF_MCMF_H
#define MCMF_MCMF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "glpk.h"


typedef struct {
    int src;
    int dst;
    double flow;
} Arc;

typedef struct {
    double rhs;
} VertexData;

typedef struct {
    double low;
    double cap;
    double cost;
    double x;
    double rc;
} ArcData;


glp_graph *createGraph();

glp_prob *createProblem();

void addVertices(glp_graph *G, int verticesNum);

void setVertexRhs(glp_graph *G, int idx, double rhs);

void addArc(glp_graph *G, int srcIdx, int dstIdx, double low, double cap, double cost);

/**
 * 将最大流转为线性问题求解. 从1开始
 */
void maxFlowGraph2LinearProblem(glp_graph *G, glp_prob *P,  const int srcIdx, const int sinkIdx);

/**
 * 将最小费用流转发为线性问题求解
 */
void minCostGraph2LinearProblem(glp_graph *G, glp_prob *P);

/**
    求解LP
*/
bool solveLP(glp_prob *P, double *optimalValue);

/**
 * 求解MCMF
 * @param srcIdx
 * @param sinkIdx
 */
bool solve(glp_graph *G, glp_prob *P, const int srcIdx, const int sinkIdx);

void freeGraph(glp_graph *G);

void freeProblem(glp_prob *P);


#endif //MCMF_MCMF_H
