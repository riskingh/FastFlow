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
    Edge() {}
    
    Edge(uint32 fromVertex, uint32 toVertex)
    : fromVertex(fromVertex), toVertex(toVertex) {}
    
    uint32 getFromVertex() const {
        return fromVertex;
    }
    
    uint32 getToVertex() const {
        return toVertex;
    }
    
    Edge reversed() const {
        return Edge(toVertex, fromVertex);
    }
    
    friend std::istream &operator >>(std::istream &stream, Edge &edge) {
        stream >> edge.fromVertex >> edge.toVertex;
        return stream;
    }
};


class FlowEdge: public Edge {
private:
    int64 capacity, flow;
public:
    FlowEdge()
    : Edge() {}
    
    ~FlowEdge() {}
    
    FlowEdge(uint32 fromVertex, uint32 toVertex, int64 capacity, int64 flow = 0)
    : Edge(fromVertex, toVertex), capacity(capacity), flow(flow) {}
    
    int64 getCapacity() {
        return capacity;
    }
    
    int64 getFlow() {
        return flow;
    }
    
    void addFlow(int64 delta) {
        flow += delta;
    }
    
    void setFlow(int64 newFlow) {
        flow = newFlow;
    }
    
    int64 getResidual() {
        return capacity - flow;
    }
    
    FlowEdge reversed() const {
        return FlowEdge(toVertex, fromVertex, 0, -flow);
    }
    
    friend std::istream &operator >>(std::istream &stream, FlowEdge &edge) {
        stream >> edge.fromVertex >> edge.toVertex >> edge.capacity;
        edge.flow = 0;
        return stream;
    }
};

#endif /* Edge_hpp */
