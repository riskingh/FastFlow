//
//  Edge.cpp
//  FastFlow
//
//  Created by Maxim Grishkin on 02/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

#include "Edge.hpp"

#include <stdio.h>
#include <iostream>

typedef int int32;
typedef unsigned int uint32;

typedef long long int64;
typedef unsigned long long uint64;


Edge::Edge() {}
    
Edge::Edge(uint32 fromVertex, uint32 toVertex)
    : fromVertex(fromVertex), toVertex(toVertex) {}
    
uint32 Edge::getFromVertex() const {
    return fromVertex;
}

uint32 Edge::getToVertex() const {
    return toVertex;
}

Edge Edge::reversed() const {
    return Edge(toVertex, fromVertex);
}

std::istream &operator >>(std::istream &stream, Edge &edge) {
    stream >> edge.fromVertex >> edge.toVertex;
    return stream;
}


FlowEdge::FlowEdge()
: Edge() {}

FlowEdge::~FlowEdge() {}

FlowEdge::FlowEdge(uint32 fromVertex, uint32 toVertex, int64 capacity, int64 flow)
: Edge(fromVertex, toVertex), capacity(capacity), flow(flow) {}

int64 FlowEdge::getCapacity() {
    return capacity;
}

int64 FlowEdge::getFlow() {
    return flow;
}

void FlowEdge::addFlow(int64 delta) {
    flow += delta;
}

void FlowEdge::setFlow(int64 newFlow) {
    flow = newFlow;
}

int64 FlowEdge::getResidual() {
    return capacity - flow;
}

FlowEdge FlowEdge::reversed() const {
    return FlowEdge(toVertex, fromVertex, 0, -flow);
}

std::istream &operator >>(std::istream &stream, FlowEdge &edge) {
    stream >> edge.fromVertex >> edge.toVertex >> edge.capacity;
    edge.flow = 0;
    return stream;
}
