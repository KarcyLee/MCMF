//
// Created by didi on 2019/7/2.
//

#ifndef MCMF_MCMF_H
#define MCMF_MCMF_H


#include <stdio.h>
#include <stdlib.h>
#include <cfloat>
#include <vector>
#include <string>
#include "glpk.h"

using namespace std;

struct Arc {
    int src = -1;
    int dst = -1;
    double flow = 0.;
};

struct VertexData {
    double rhs;
};

struct ArcData {
    double low;
    double cap;
    double cost;
    double x;
    double rc;

};


const int GLP_A_CAP = offsetof(ArcData, cap);
const int GLP_A_COST = offsetof(ArcData, cost);
const int GLP_A_LOW = offsetof(ArcData, low);
const int GLP_A_RC = offsetof(ArcData, rc);
const int GLP_A_X = offsetof(ArcData, x);
const int GLP_A_SIZE = sizeof(ArcData);


const int GLP_V_RHS = offsetof(VertexData, rhs);
const int GLP_V_SIZE = sizeof(VertexData);


class MCMF {
public:
    MCMF() {
        G = glp_create_graph(sizeof(VertexData), sizeof(ArcData));
    }

    void addVertices(int verticesNum) {
        int ret = glp_add_vertices(G, verticesNum);
        for (int i = 1; i <= verticesNum; ++i) {
            //glp_set_vertex_name(G, i, "v" + i);
            glp_set_vertex_name(G, i, ("v" + to_string(i)).c_str());

        }
    }

    void setVertexRhs(int idx, double rhs) {
        getVertexData(idx)->rhs = rhs;
    }

    void addArc(int srcIdx, int dstIdx, double low, double cap, int cost) {
        glp_arc *arc = glp_add_arc(G, srcIdx, dstIdx);
        ArcData *ad = getArcData(arc);
        ad->low = low;
        ad->cap = cap;
        ad->cost = cost;
    }

    /**
     * 求解MCMF
     * @param srcIdx
     * @param sinkIdx
     */
    vector<Arc> solve(const int srcIdx, const int sinkIdx) {
        ok = false;

        vector<Arc> arcs;

        maxFlowGraph2LinearProblem(srcIdx, sinkIdx);

        bool solveStatus = solveLP();

        glp_print_sol(P, "mf.sol");

        if (solveStatus) {
            ok = false;
            double maxFlow = optimalValue;
            printf("MAX FLOW||OV=%4.f\n", optimalValue);

            setVertexRhs(srcIdx, maxFlow);
            setVertexRhs(sinkIdx, -maxFlow);

            minCostGraph2LinearProblem();
            solveStatus = solveLP();

            if (solveStatus) {
                ok = true;
                arcs = getResultArcs();
                //printf("MCMF||succeed\n");
                glp_print_sol(P, "mcmf.sol");

            } else {
                printf("MCMF||max flow succeed||min cost fail\n");
            }

        } else {
            printf("MCMF||max flow fail\n");
        }

        //free();
        return arcs;
    }


    /**
     * 解析求解结果中的弧
     */
    vector<Arc> getResultArcs() {
        vector<Arc> ret;
        if (!ok) {
            return ret;
        }

        for (int i = 0; i < glp_get_num_cols(P); ++i) {
            string cn = glp_get_col_name(P, i + 1);
            Arc arc = colName2Arc(cn);
            if (arc.src == -1 || arc.dst == -1) {
                continue;
            }
            arc.flow = glp_get_col_prim(P, i + 1);
            ret.push_back(arc);
        }
        return ret;
    }

    double getOptimalValue() {
        if (ok) {
            return optimalValue;
        }
        return DBL_MIN;
    }

    ~MCMF() {
        free();
    }

private:
    glp_graph *G;
    glp_prob *P;
    double optimalValue = DBL_MAX;
    bool ok = false;

    VertexData *getVertexData(int idx) {
        return (VertexData *) (G->v[idx]->data);
    }

    ArcData *getArcData(glp_arc *arc) {
        return (ArcData *) (arc->data);
    }

    /**
     * 将最大流转为线性问题求解. 从1开始
     */
    void maxFlowGraph2LinearProblem(const int srcIdx, const int sinkIdx) {
        if (G == nullptr) {
            P = nullptr;
            return;
        }
        if (P != nullptr) {
            glp_delete_prob(P);
        }

        P = glp_create_prob();
        glp_maxflow_lp(P, G, GLP_ON, srcIdx, sinkIdx, GLP_A_CAP);


        glp_write_lp(P, nullptr, "maxFlow.lp");
        glp_write_maxflow(G, srcIdx, sinkIdx, GLP_A_COST, "maxflow.dimacs");
    }

    /**
     * 将最小费用流转发为线性问题求解
     */
    void minCostGraph2LinearProblem() {
        if (G == nullptr) {
            P = nullptr;
            return;
        }
        if (P != nullptr) {
            glp_delete_prob(P);
        }

        P = glp_create_prob();
        glp_mincost_lp(P, G, GLP_ON, GLP_V_RHS, GLP_A_LOW, GLP_A_CAP, GLP_A_COST);

        //test
        glp_write_lp(P, nullptr, "minCost.lp");
        glp_write_mincost(G, GLP_V_RHS, GLP_A_LOW, GLP_A_CAP, GLP_A_COST, "mincost.dimacs");
    }

    /**
        求解LP
    */
    bool solveLP() {
        auto parm = new glp_smcp();
        glp_init_smcp(parm);
        parm->msg_lev = GLP_MSG_OFF;
        parm->presolve = GLP_ON;

        int ret = glp_simplex(P, parm);
        if (ret == 0) {
            optimalValue = glp_get_obj_val(P);
            printf("optimalValue:%2.f\n", optimalValue);
        }
        return ret == 0;
    }

    Arc colName2Arc(const string colName) {
        Arc arc;
        if (colName == "") {
            return arc;
        }

        if (colName[0] != 'x' || colName[1] != '[' || colName[colName.length() - 1] != ']') {
            return arc;
        }

        string name = colName.substr(2, colName.length() - 2);

        vector<string> arr = split(name, ",");

        int src = std::stoi(arr[0]);
        int dst = std::stoi(arr[1]);

        arc.src = src;
        arc.dst = dst;
        return arc;

    }


    vector<string> split(const string &str, const string &delim) {
        vector<string> res;
        if ("" == str) return res;
        //先将要切割的字符串从string类型转换为char*类型
        char *strs = new char[str.length() + 1]; //不要忘了
        strcpy(strs, str.c_str());

        char *d = new char[delim.length() + 1];
        strcpy(d, delim.c_str());

        char *p = strtok(strs, d);
        while (p) {
            string s = p; //分割得到的字符串转换为string类型
            res.push_back(s); //存入结果数组
            p = strtok(NULL, d);
        }

        return res;
    }

    void free() {
        if (G != nullptr) {
            glp_delete_graph(G);
        }
        if (P != nullptr) {
            glp_delete_prob(P);
        }
    }
};


#endif //MCMF_MCMF_H
