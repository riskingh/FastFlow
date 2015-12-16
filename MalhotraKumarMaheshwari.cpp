//
//  MalhotraKumarMaheshwari.cpp
//  FastFlow
//
//  Created by Maxim Grishkin on 13/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

#include "MalhotraKumarMaheshwari.hpp"

    
int64 MalhotraKumarMaheshwari::potential(uint32 vertex) {
    return std::min(inFlow[vertex], outFlow[vertex]);
}

void MalhotraKumarMaheshwari::deleteVertex(uint32 vertex) {
    //        FIX HERE
    level[vertex] = ULLONG_MAX;
    FlowGraph::iterator edgeIterator;
    uint32 nextVertex;
    for (edgeIterator = graph->begin(vertex); edgeIterator != graph->end(vertex); ++edgeIterator) {
        nextVertex = edgeIterator->getToVertex();
        if (level[vertex] + 1 == level[nextVertex])
            inFlow[nextVertex] -= edgeIterator->getResidual();
        if (level[vertex] == level[nextVertex] + 1)
            outFlow[nextVertex] -= edgeIterator.oppositeEdge().getResidual();
        if (level[nextVertex] != ULLONG_MAX && potential(nextVertex) == 0)
            deleteVertex(nextVertex);
    }
}

bool MalhotraKumarMaheshwari::calculateLevels() {
    bfs(graph, source, level, flowEdgeExists);
    return level[sink] != ULLONG_MAX;
}

void MalhotraKumarMaheshwari::deleteZeroPotential() {
    for (uint32 vertex = 1; vertex <= graph->size(); ++vertex) {
        if (potential(vertex) == 0)
            deleteVertex(vertex);
    }
}

void MalhotraKumarMaheshwari::calculatePotentials() {
    inFlow.assign(graph->size() + 1, 0);
    outFlow.assign(graph->size() + 1, 0);
    
    uint32 vertex, nextVertex;
    FlowGraph::iterator edgeIterator;
    for (vertex = 1; vertex <= graph->size(); ++vertex) {
        for (edgeIterator = graph->begin(vertex); edgeIterator != graph->end(vertex); ++edgeIterator) {
            nextVertex = edgeIterator->getToVertex();
            if (level[vertex] + 1 == level[nextVertex]) {
                outFlow[vertex] += edgeIterator->getResidual();
                inFlow[nextVertex] += edgeIterator->getResidual();
            }
        }
    }
    
    inFlow[source] = outFlow[sink] = LLONG_MAX;
    deleteZeroPotential();
}

uint32 MalhotraKumarMaheshwari::getReferenceVertex() {
    uint32 minPotentialVertex = UINT32_MAX;
    int64 minPotential = LLONG_MAX;
    
    for (uint32 vertex = 1; vertex <= graph->size(); ++vertex) {
        if (level[vertex] != ULLONG_MAX && potential(vertex) <= minPotential) {
            minPotentialVertex = vertex;
            minPotential = potential(vertex);
        }
    }
    
    return minPotentialVertex;
}

bool MalhotraKumarMaheshwari::rightDirection(uint64 fromLevel, uint64 toLevel, PushDirection direction) {
    if (direction == ToSource)
        return fromLevel == toLevel + 1;
    else
        return fromLevel + 1 == toLevel;
}

int64 MalhotraKumarMaheshwari::getDirectionalResidual(FlowGraph::iterator edgeIterator, PushDirection direction) {
    if (direction == ToSink)
        return edgeIterator->getResidual();
    else
        return edgeIterator.oppositeEdge().getResidual();
}

int64 MalhotraKumarMaheshwari::directionalPush(int64 push, PushDirection direction) {
    return push * (direction == ToSink ? 1 : -1);
}

int64 MalhotraKumarMaheshwari::pushFlow(uint32 vertex, int64 flow, PushDirection direction) {
    if (vertex == source && direction == ToSource)
        return flow;
    if (vertex == sink && direction == ToSink)
        return flow;
    
    int64 pushed = 0, residual;
    uint32 nextVertex;
    for (; flow && currentEdgeIterator[vertex] != graph->end(vertex); ++currentEdgeIterator[vertex]) {
        nextVertex = currentEdgeIterator[vertex]->getToVertex();
        if (rightDirection(level[vertex], level[nextVertex], direction)) {
            residual = getDirectionalResidual(currentEdgeIterator[vertex], direction);
            pushed += pushFlow(nextVertex, std::min(flow, residual), direction);
            flow -= pushed;
            if (direction == ToSink) {
                outFlow[vertex] -= pushed;
                inFlow[nextVertex] -= pushed;
            }
            else {
                inFlow[vertex] -= pushed;
                outFlow[nextVertex] -= pushed;
            }
            currentEdgeIterator[vertex]->addFlow(directionalPush(pushed, direction));
            currentEdgeIterator[vertex].oppositeEdge().addFlow(-directionalPush(pushed, direction));
        }
    }
    return pushed;
}

void MalhotraKumarMaheshwari::pushFlow(uint32 vertex) {
    int64 currentPotential = potential(vertex);
    pushFlow(vertex, currentPotential, ToSource);
    pushFlow(vertex, currentPotential, ToSink);
    deleteZeroPotential();
}


MalhotraKumarMaheshwari::MalhotraKumarMaheshwari(FlowGraph *graph, uint32 source, uint32 sink)
: graph(graph), source(source), sink(sink) {
    inFlow.resize(graph->size() + 1);
    outFlow.resize(graph->size() + 1);
    level.resize(graph->size() + 1);
    currentEdgeIterator.resize(graph->size() + 1);
}

MalhotraKumarMaheshwari::~MalhotraKumarMaheshwari() {
    inFlow.clear();
    outFlow.clear();
    level.clear();
    currentEdgeIterator.clear();
}

int64 MalhotraKumarMaheshwari::getFlow() {
    int64 flow = 0;
    uint32 vertex;
    while (calculateLevels()) {
        calculatePotentials();
        for (vertex = 1; vertex <= graph->size(); ++vertex)
            currentEdgeIterator[vertex] = graph->begin(vertex);
        vertex = getReferenceVertex();
        for (; vertex != UINT32_MAX && potential(vertex) != 0 ; vertex = getReferenceVertex()) {
            flow += potential(vertex);
            pushFlow(vertex);
        }
    }
    return flow;
}
