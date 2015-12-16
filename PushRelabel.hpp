//
//  PushRelabel.hpp
//  FastFlow
//
//  Created by Maxim Grishkin on 02/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

#ifndef PushRelabel_hpp
#define PushRelabel_hpp

#include <stdio.h>
#include <vector>
#include <list>

#include "Edge.hpp"
#include "Graph.hpp"

typedef int int32;
typedef unsigned int uint32;

typedef long long int64;
typedef unsigned long long uint64;


class PushRelabel {
private:
    FlowGraph *graph;
    
    uint32 source, sink;

    std::vector< uint32 > current;
    std::vector< int64 > excess;
    std::vector< uint32 > height;
    std::list< uint32 > vertices;
    
    FlowEdge &getCurrentEdge(uint32 vertex) const;
    FlowEdge &getOppositeCurrentEdge(uint32 vertex) const;
    bool push(uint32 vertex);
    void relabel(uint32 vertex);
    void discharge(uint32 vertex);
    void findFlow();

public:
    PushRelabel(FlowGraph *graph, uint32 source, uint32 sink);
    ~PushRelabel();
    int64 getFlow();
};

#include "PushRelabel.cpp"

#endif /* PushRelabel_hpp */
