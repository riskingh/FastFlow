//
//  Graph.hpp
//  FastFlow
//
//  Created by Maxim Grishkin on 02/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

#include "Edge.hpp"

typedef int int32;
typedef unsigned int uint32;

typedef long long int64;
typedef unsigned long long uint64;


template <class TEdge>
class Graph {
protected:
    std::vector<TEdge> edges;
    std::vector< std::vector< uint32 > > adjacent;
    
public:
    Graph() {}
    
    friend std::istream &operator >>(std::istream &stream, Graph &graph) {
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
    
    class iterator {
    private:
        Graph *graph;
        std::vector< uint32 >::iterator edgeNumberIterator;
        
    public:
        iterator () {}
        
        iterator(Graph *graph, std::vector< uint32 >::iterator edgeNumberIterator)
        : graph(graph), edgeNumberIterator(edgeNumberIterator) {}
        
        iterator &operator ++() {
            ++edgeNumberIterator;
            return *this;
        }
        
        iterator operator ++(int32) {
            iterator old(*this);
            ++edgeNumberIterator;
            return old;
        }
        
        TEdge &operator *() {
            return graph->edges[*edgeNumberIterator];
        }
        
        TEdge &oppositeEdge() {
            return graph->edges[(*edgeNumberIterator) ^ 1];
        }
        
        TEdge *operator ->() {
            return &graph->edges[*edgeNumberIterator];
        }
        
        bool operator ==(iterator other) {
            return graph == other.graph && edgeNumberIterator == other.edgeNumberIterator;
        }
        
        bool operator !=(iterator other) {
            return graph != other.graph || edgeNumberIterator != other.edgeNumberIterator;
        }
    };
    
    iterator begin(uint32 vertex) {
        return iterator(this, adjacent[vertex].begin());
    }
    
    iterator end(uint32 vertex) {
        return iterator(this, adjacent[vertex].end());
    }
    
    uint32 size() {
        return (int32)adjacent.size() - 1;
    }
    
    uint32 size(uint32 vertex) {
        return (uint32)adjacent[vertex].size();
    }
    
    TEdge &getEdge(uint32 vertex, uint32 edgeNumber) {
        return edges[adjacent[vertex][edgeNumber]];
    }

    TEdge &getOppositeEdge(uint32 vertex, uint32 edgeNumber) {
        return edges[adjacent[vertex][edgeNumber] ^ 1];
    }
};


class FlowGraph: public Graph<FlowEdge> {
public:
    FlowGraph() {}
    
    friend std::istream &operator >>(std::istream &stream, FlowGraph &graph) {
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
};

#endif /* Graph_hpp */
