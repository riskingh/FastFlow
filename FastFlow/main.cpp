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
#include "PushRelabel.hpp"
#include "MalhotraKumarMaheshwari.hpp"

int main(int argc, const char * argv[]) {
    freopen("input.txt", "r", stdin);
    FlowGraph g;
    std::cin >> g;
//    std::cout << doPushRelabel(&g, 1, g.size()) << "\n";
    std::cout << doMalhotraKumarMaheshwari(&g, 1, g.size()) << "\n";
    return 0;
}
