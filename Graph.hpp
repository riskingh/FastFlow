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
    Graph();
    friend std::istream &operator >>(std::istream &stream, Graph &graph);
    
    class iterator {
    private:
        Graph *graph;
        std::vector< uint32 >::iterator edgeNumberIterator;
        
    public:
        iterator ();
        iterator(Graph *graph, std::vector< uint32 >::iterator edgeNumberIterator);
        iterator &operator ++();
        iterator operator ++(int32);
        TEdge &operator *();
        TEdge &oppositeEdge();
        TEdge *operator ->();
        bool operator ==(iterator other);
        bool operator !=(iterator other);
    };
    
    iterator begin(uint32 vertex);
    iterator end(uint32 vertex);
    uint32 size();
    uint32 size(uint32 vertex);
    TEdge &getEdge(uint32 vertex, uint32 edgeNumber);
    TEdge &getOppositeEdge(uint32 vertex, uint32 edgeNumber);
};


class FlowGraph: public Graph<FlowEdge> {
public:
    FlowGraph();
    friend std::istream &operator >>(std::istream &stream, FlowGraph &graph);
};

#include "Graph.cpp"

#endif /* Graph_hpp */
