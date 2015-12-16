//
//  GraphFunctions.cpp
//  FastFlow
//
//  Created by Maxim Grishkin on 16/12/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

template <class TEdge>
bool defaultEdgeExist(TEdge &edge) {
    return true;
}

bool flowEdgeExists(FlowEdge &edge) {
    return edge.getResidual() > 0;
}

template <class TEdge>
void bfs(Graph<TEdge> *graph, uint32 start, std::vector<uint64> &level, bool (*edgeExist)(TEdge &edge)) {
    level.assign(graph->size() + 1, ULLONG_MAX);
    level[start] = 0;
    std::queue<uint32> bfsQueue;
    bfsQueue.push(start);
    
    uint32 vertex, nextVertex;
    FlowGraph::iterator edgeIterator;
    while (!bfsQueue.empty()) {
        vertex = bfsQueue.front();
        bfsQueue.pop();
        for (edgeIterator = graph->begin(vertex); edgeIterator != graph->end(vertex); ++edgeIterator) {
            if (!edgeExist(*edgeIterator))
                continue;
            nextVertex = edgeIterator->getToVertex();
            if (level[nextVertex] == ULLONG_MAX) {
                level[nextVertex] = level[vertex] + 1;
                bfsQueue.push(nextVertex);
            }
        }
    }
}