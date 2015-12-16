//
//  PushRelabel.cpp
//  FastFlow
//
//  Created by Maxim Grishkin on 02/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

//#include "PushRelabel.hpp"

    
FlowEdge &PushRelabel::getCurrentEdge(uint32 vertex) const {
    return graph->getEdge(vertex, current[vertex]);
}

FlowEdge &PushRelabel::getOppositeCurrentEdge(uint32 vertex) const {
    return graph->getOppositeEdge(vertex, current[vertex]);
}

bool PushRelabel::push(uint32 vertex) {
    FlowEdge &edge = getCurrentEdge(vertex), &oppositeEdge = getOppositeCurrentEdge(vertex);
    if (edge.getResidual() <= 0 || height[edge.getFromVertex()] <= height[edge.getToVertex()])
        return false;
    
    int64 delta = std::min(excess[vertex], edge.getResidual());
    
    excess[edge.getFromVertex()] -= delta;
    excess[edge.getToVertex()] += delta;
    edge.addFlow(delta);
    oppositeEdge.addFlow(-delta);
    return true;
}

void PushRelabel::relabel(uint32 vertex) {
    current[vertex] = 0;
    
    uint32 minHeight = -1;
    
    FlowGraph::iterator edgeIterator;
    for (edgeIterator = graph->begin(vertex); edgeIterator != graph->end(vertex); ++edgeIterator)
        if (edgeIterator->getResidual() > 0)
            minHeight = std::min(minHeight, height[edgeIterator->getToVertex()]);
    
    minHeight = minHeight == -1 ? height[vertex] : minHeight;
    height[vertex] = minHeight + 1;
}

void PushRelabel::discharge(uint32 vertex) {
    while (excess[vertex] > 0) {
        if (current[vertex] == graph->size(vertex))
            relabel(vertex);
        else if (push(vertex))
            ;
        else
            ++current[vertex];
    }
}

void PushRelabel::findFlow() {
    std::list< uint32 >::iterator vertex;
    uint32 oldHeight, vertexNumber;
    for (vertex = vertices.begin(); vertex != vertices.end(); vertex++) {
        oldHeight = height[*vertex];
        discharge(*vertex);
        if (oldHeight != height[*vertex]) {
            vertexNumber = *vertex;
            vertices.erase(vertex);
            vertices.push_front(vertexNumber);
            vertex = vertices.begin();
        }
    }
}


PushRelabel::PushRelabel(FlowGraph *graph, uint32 source, uint32 sink)
: graph(graph), source(source), sink(sink) {
    current.resize(graph->size() + 1, 0);
    excess.resize(graph->size() + 1, 0);
    height.resize(graph->size() + 1, 0);
    height[source] = graph->size();
    for (uint32 vertex = 1; vertex <= graph->size(); ++vertex) {
        if (vertex != source && vertex != sink)
            vertices.push_back(vertex);
    }
    
    FlowGraph::iterator edgeIterator;
    for (edgeIterator = graph->begin(source); edgeIterator != graph->end(source); ++edgeIterator) {
        edgeIterator->setFlow(edgeIterator->getCapacity());
        edgeIterator.oppositeEdge().setFlow(-edgeIterator->getCapacity());
        excess[edgeIterator->getToVertex()] += edgeIterator->getCapacity();
    }
}

PushRelabel::~PushRelabel() {
    current.clear();
    excess.clear();
    height.clear();
    vertices.clear();
}

int64 PushRelabel::getFlow() {
    findFlow();
    int64 flow = 0;
    FlowGraph::iterator edgeIterator;
    for (edgeIterator = graph->begin(source); edgeIterator != graph->end(source); ++edgeIterator)
        flow += (*edgeIterator).getFlow();
    return flow;
}
