//
//  GraphFunctions.hpp
//  FastFlow
//
//  Created by Maxim Grishkin on 16/12/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

#ifndef GraphFunctions_hpp
#define GraphFunctions_hpp

#include <stdio.h>
#include <vector>
#include <queue>
#include <climits>

typedef int int32;
typedef unsigned int uint32;

typedef long long int64;
typedef unsigned long long uint64;

template <class TEdge>
bool defaultEdgeExist(TEdge &edge);

bool flowEdgeExists(FlowEdge &edge);

template <class TEdge>
void bfs(Graph<TEdge> *graph, uint32 start, std::vector<uint64> &level, bool (*edgeExist)(TEdge &edge) = defaultEdgeExist);

#include "GraphFunctions.cpp"

#endif /* GraphFunctions_hpp */
