//
//  MalhotraKumarMaheshwari.hpp
//  FastFlow
//
//  Created by Maxim Grishkin on 13/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

#ifndef MalhotraKumarMaheshwari_hpp
#define MalhotraKumarMaheshwari_hpp

#include <stdio.h>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>

#include "Edge.hpp"
#include "Graph.hpp"
#include "GraphFunctions.hpp"

typedef int int32;
typedef unsigned int uint32;

typedef long long int64;
typedef unsigned long long uint64;

enum PushDirection {
    ToSource,
    ToSink
};


class MalhotraKumarMaheshwari {
private:
    FlowGraph *graph;
    uint32 source, sink;
    
    std::vector< int64 > inFlow, outFlow;
    std::vector< uint64 > level;
    
    std::vector< FlowGraph::iterator > currentEdgeIterator;

    
    int64 potential(uint32 vertex);
    void deleteVertex(uint32 vertex);
    bool calculateLevels();
    void deleteZeroPotential();
    void calculatePotentials();
    uint32 getReferenceVertex();
    
    bool rightDirection(uint64 fromLevel, uint64 toLevel, PushDirection direction);
    int64 getDirectionalResidual(FlowGraph::iterator edgeIterator, PushDirection direction);
    int64 directionalPush(int64 push, PushDirection direction);

    int64 pushFlow(uint32 vertex, int64 flow, PushDirection direction);
    void pushFlow(uint32 vertex);
    
public:
    MalhotraKumarMaheshwari(FlowGraph *graph, uint32 source, uint32 sink);
    
    ~MalhotraKumarMaheshwari();

    int64 getFlow();
};

#endif /* MalhotraKumarMaheshwari_hpp */
