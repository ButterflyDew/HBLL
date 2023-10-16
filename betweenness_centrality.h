#pragma once
#include "global.h"
#include <cstdio>
class B_C
{
public:
    int n;
    vector <double> BC;
    void sol(int rt, Graph &G);

    void build(Graph G);
};