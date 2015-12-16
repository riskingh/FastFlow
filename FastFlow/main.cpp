//
//  main.cpp
//  FastFlow
//
//  Created by Maxim Grishkin on 02/11/15.
//  Copyright © 2015 Maxim Grishkin. All rights reserved.
//

#include <iostream>

#include "Edge.hpp"
#include "Graph.hpp"
#include "GraphFunctions.hpp"
#include "PushRelabel.hpp"
#include "MalhotraKumarMaheshwari.hpp"

int main(int argc, const char * argv[]) {
    freopen("input.txt", "r", stdin);
    FlowGraph graph;
    std::cin >> graph;
    MalhotraKumarMaheshwari temp(&graph, 0, (int)graph.size() - 1);
    std::cout << "flow: " << temp.getFlow() << "\n";
    return 0;
}
