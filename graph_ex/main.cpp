// main.cpp
#include "Algorithms.cpp"
#include <iostream>

int main() {
    std::cout << "Graph Implementation Demonstration" << std::endl;
    std::cout << "===================================" << std::endl;
    
    // Create a graph
    std::cout << "\nCreating a graph with 6 vertices:" << std::endl;
    graph::Graph g(6);
    
    // Add edges
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 5);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 7);
    g.addEdge(2, 4, 8);
    g.addEdge(3, 4, 6);
    g.addEdge(3, 5, 1);
    g.addEdge(4, 5, 9);
    
    // Print the graph
    std::cout << "\nOriginal graph:" << std::endl;
    g.print_graph();
    
    // BFS traversal
    std::cout << "\nBFS traversal starting from vertex 0:" << std::endl;
    graph::Graph bfsTree = graph::Algorithms::bfs(g, 0);
    bfsTree.print_graph();
    
    // DFS traversal
    std::cout << "\nDFS traversal starting from vertex 0:" << std::endl;
    graph::Graph dfsTree = graph::Algorithms::dfs(g, 0);
    dfsTree.print_graph();
    
    // Dijkstra's shortest paths
    std::cout << "\nShortest paths from vertex 0 (Dijkstra's algorithm):" << std::endl;
    graph::Graph shortestPaths = graph::Algorithms::dijkstra(g, 0);
    shortestPaths.print_graph();
    
    // Prim's MST
    std::cout << "\nMinimum Spanning Tree (Prim's algorithm):" << std::endl;
    graph::Graph primMST = graph::Algorithms::prim(g);
    primMST.print_graph();
    
    // Kruskal's MST
    std::cout << "\nMinimum Spanning Tree (Kruskal's algorithm):" << std::endl;
    graph::Graph kruskalMST = graph::Algorithms::kruskal(g);
    kruskalMST.print_graph();
    
    return 0;
}