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

    
    int64 potential(uint32 vertex) {
        return std::min(inFlow[vertex], outFlow[vertex]);
    }
    
    void deleteVertex(uint32 vertex) {
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
    
    bool calculateLevels() {
        level.assign(graph->size() + 1, ULLONG_MAX);
        level[source] = 0;
        std::queue<uint32> bfsQueue;
        bfsQueue.push(source);
        
        uint32 vertex, nextVertex;
        FlowGraph::iterator edgeIterator;
        while (!bfsQueue.empty()) {
            vertex = bfsQueue.front();
            bfsQueue.pop();
            for (edgeIterator = graph->begin(vertex); edgeIterator != graph->end(vertex); ++edgeIterator) {
                nextVertex = edgeIterator->getToVertex();
                if (level[nextVertex] == ULLONG_MAX) {
                    level[nextVertex] = level[vertex] + 1;
                    bfsQueue.push(nextVertex);
                }
            }
        }
        
        return level[sink] != ULLONG_MAX;
    }
    
    void calculatePotentials() {
        inFlow.assign(graph->size(), 0);
        outFlow.assign(graph->size(), 0);
        
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
        
        for (vertex = 1; vertex <= graph->size(); ++vertex) {
            if (potential(vertex) == 0)
                deleteVertex(vertex);
        }
    }

    uint32 getReferenceVertex() {
        uint32 minPotentialVertex = UINT32_MAX;
        int64 minPotential = LLONG_MAX;
        
        for (uint32 vertex = 1; vertex <= graph->size(); ++vertex) {
            if (potential(vertex) <= minPotential) {
                minPotentialVertex = vertex;
                minPotential = potential(vertex);
            }
        }
        
        return minPotentialVertex;
    }
    
    bool rightDirection(uint64 fromLevel, uint64 toLevel, PushDirection direction) {
        if (direction == ToSource)
            return fromLevel == toLevel + 1;
        else
            return fromLevel + 1 == toLevel;
    }

    int64 pushFlow(uint32 vertex, int64 flow, PushDirection direction) {
        if (vertex == source && direction == ToSource)
            return flow;
        if (vertex == sink && direction == ToSink)
            return flow;
        
        int64 pushed = 0;
        uint32 nextVertex;
        for (; flow && currentEdgeIterator[vertex] != graph->end(vertex); ++currentEdgeIterator[vertex]) {
            nextVertex = currentEdgeIterator[vertex]->getToVertex();
            if (rightDirection(level[vertex], level[nextVertex], direction)) {
                pushed += pushFlow(nextVertex, std::min(flow, currentEdgeIterator[vertex]->getResidual()), direction);
                flow -= pushed;
                outFlow[vertex] -= pushed;
                inFlow[nextVertex] -= pushed;
                currentEdgeIterator[vertex]->addFlow(pushed);
                if (potential(nextVertex) == 0)
                    deleteVertex(nextVertex);
            }
        }
        return pushed;
    }
    
    void pushFlow(uint32 vertex) {
        pushFlow(vertex, potential(vertex), ToSource);
        pushFlow(vertex, potential(vertex), ToSink);
    }
    
public:
    MalhotraKumarMaheshwari(FlowGraph *graph, uint32 source, uint32 sink)
    : graph(graph), source(source), sink(sink) {
        inFlow.resize(graph->size() + 1);
        outFlow.resize(graph->size() + 1);
        level.resize(graph->size() + 1);
        currentEdgeIterator.resize(graph->size() + 1);
    }
    
    ~MalhotraKumarMaheshwari() {
        inFlow.clear();
        outFlow.clear();
        level.clear();
        currentEdgeIterator.clear();
    }

    int64 getFlow() {
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
                deleteVertex(vertex);
            }
        }
        return flow;
    }
};

int64 doMalhotraKumarMaheshwari(FlowGraph *graph, uint32 source, uint32 sink) {
    MalhotraKumarMaheshwari *temp = new MalhotraKumarMaheshwari(graph, source, sink);
    int64 flow = temp->getFlow();
    delete temp;
    return flow;
}

#endif /* MalhotraKumarMaheshwari_hpp */
