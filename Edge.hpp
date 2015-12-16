//
//  Edge.hpp
//  FastFlow
//
//  Created by Maxim Grishkin on 02/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

#ifndef Edge_hpp
#define Edge_hpp

#include <stdio.h>
#include <iostream>

typedef int int32;
typedef unsigned int uint32;

typedef long long int64;
typedef unsigned long long uint64;


class Edge {
protected:
    uint32 fromVertex, toVertex;
public:
    Edge();
    Edge(uint32 fromVertex, uint32 toVertex);
    uint32 getFromVertex() const;
    uint32 getToVertex() const;
    Edge reversed() const;
    
    friend std::istream &operator >>(std::istream &stream, Edge &edge);
};


class FlowEdge: public Edge {
private:
    int64 capacity, flow;
public:
    FlowEdge();
    ~FlowEdge();
    FlowEdge(uint32 fromVertex, uint32 toVertex, int64 capacity, int64 flow = 0);
    
    int64 getCapacity();
    int64 getFlow();
    void addFlow(int64 delta);
    void setFlow(int64 newFlow);
    int64 getResidual();
    FlowEdge reversed() const;
    
    friend std::istream &operator >>(std::istream &stream, FlowEdge &edge);
};

#endif /* Edge_hpp */
