//
//  Graph.cpp
//  FastFlow
//
//  Created by Maxim Grishkin on 02/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

template <class TEdge>
Graph<TEdge>::Graph() {}

template <class TEdge>
std::istream &operator >>(std::istream &stream, Graph<TEdge> &graph) {
    uint32 vertexNumber, edgeNumber;
    stream >> vertexNumber >> edgeNumber;
    
    graph.adjacent.resize(vertexNumber + 1);
    
    TEdge edge;
    for (; edgeNumber > 0; --edgeNumber) {
        stream >> edge;
        graph.adjacent[edge.getFromVertex()].push_back((uint32)graph.edges.size());
        graph.edges.push_back(edge);
    }
    
    return stream;
}

template <class TEdge>
Graph<TEdge>::iterator::iterator () {}

template <class TEdge>
Graph<TEdge>::iterator::iterator(Graph<TEdge> *graph, std::vector< uint32 >::iterator edgeNumberIterator)
: graph(graph), edgeNumberIterator(edgeNumberIterator) {}

template <class TEdge>
typename Graph<TEdge>::iterator &Graph<TEdge>::iterator::operator ++() {
    ++edgeNumberIterator;
    return *this;
}

template <class TEdge>
typename Graph<TEdge>::iterator Graph<TEdge>::iterator::operator ++(int32) {
    iterator old(*this);
    ++edgeNumberIterator;
    return old;
}

template <class TEdge>
TEdge &Graph<TEdge>::iterator::operator *() {
    return graph->edges[*edgeNumberIterator];
}

template <class TEdge>
TEdge &Graph<TEdge>::iterator::oppositeEdge() {
    return graph->edges[(*edgeNumberIterator) ^ 1];
}

template <class TEdge>
TEdge *Graph<TEdge>::iterator::operator ->() {
    return &graph->edges[*edgeNumberIterator];
}

template <class TEdge>
bool Graph<TEdge>::iterator::operator ==(iterator other) {
    return graph == other.graph && edgeNumberIterator == other.edgeNumberIterator;
}

template <class TEdge>
bool Graph<TEdge>::iterator::operator !=(iterator other) {
    return graph != other.graph || edgeNumberIterator != other.edgeNumberIterator;
}

template <class TEdge>
typename Graph<TEdge>::iterator Graph<TEdge>::begin(uint32 vertex) {
    return iterator(this, adjacent[vertex].begin());
}

template <class TEdge>
typename Graph<TEdge>::iterator Graph<TEdge>::end(uint32 vertex) {
    return iterator(this, adjacent[vertex].end());
}

template <class TEdge>
uint32 Graph<TEdge>::size() {
    return (int32)adjacent.size() - 1;
}

template <class TEdge>
uint32 Graph<TEdge>::size(uint32 vertex) {
    return (uint32)adjacent[vertex].size();
}

template <class TEdge>
TEdge &Graph<TEdge>::getEdge(uint32 vertex, uint32 edgeNumber) {
    return edges[adjacent[vertex][edgeNumber]];
}

template <class TEdge>
TEdge &Graph<TEdge>::getOppositeEdge(uint32 vertex, uint32 edgeNumber) {
    return edges[adjacent[vertex][edgeNumber] ^ 1];
}


FlowGraph::FlowGraph() {}

std::istream &operator >>(std::istream &stream, FlowGraph &graph) {
    uint32 vertexNumber, edgeNumber;
    stream >> vertexNumber >> edgeNumber;
    
    graph.adjacent.resize(vertexNumber + 1);
    
    FlowEdge edge;
    for (; edgeNumber > 0; --edgeNumber) {
        stream >> edge;
        graph.adjacent[edge.getFromVertex()].push_back((uint32)graph.edges.size());
        graph.edges.push_back(edge);
        graph.adjacent[edge.getToVertex()].push_back((uint32)graph.edges.size());
        graph.edges.push_back(edge.reversed());
    }
    
    return stream;
}