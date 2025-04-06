// tests.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Algorithms.cpp"
#include <iostream>

// Helper functions
int countEdges(const graph::Graph& g) {
    int count = 0;
    for (int i = 0; i < g.getNumVertices(); i++) {
        graph::Graph::Edge* edge = g.getAdjList(i);
        while (edge) {
            if (i < edge->destination) count++; // Count each edge only once
            edge = edge->next;
        }
    }
    return count;
}

int totalWeight(const graph::Graph& g) {
    int sum = 0;
    for (int i = 0; i < g.getNumVertices(); i++) {
        graph::Graph::Edge* edge = g.getAdjList(i);
        while (edge) {
            if (i < edge->destination) sum += edge->weight;
            edge = edge->next;
        }
    }
    return sum;
}

bool hasEdge(const graph::Graph& g, int src, int dest) {
    graph::Graph::Edge* edge = g.getAdjList(src);
    while (edge) {
        if (edge->destination == dest) return true;
        edge = edge->next;
    }
    return false;
}

bool isConnected(const graph::Graph& g) {
    int n = g.getNumVertices();
    if (n == 0) return true;
    
    bool* visited = new bool[n]();
    graph::Queue q;
    q.enqueue(0);
    visited[0] = true;
    
    while (!q.isEmpty()) {
        int v = q.dequeue();
        graph::Graph::Edge* edge = g.getAdjList(v);
        while (edge) {
            if (!visited[edge->destination]) {
                visited[edge->destination] = true;
                q.enqueue(edge->destination);
            }
            edge = edge->next;
        }
    }
    
    bool allVisited = true;
    for (int i = 0; i < n && allVisited; i++)
        if (!visited[i]) allVisited = false;
    
    delete[] visited;
    return allVisited;
}

TEST_CASE("Basic Graph Operations") {
    graph::Graph g(4);
    
    CHECK(g.getNumVertices() == 4);
    
    g.addEdge(0, 1, 5);
    g.addEdge(1, 2, 3);
    g.addEdge(2, 3, 1);
    g.addEdge(0, 3, 10);
    
    CHECK(hasEdge(g, 0, 1));
    CHECK(hasEdge(g, 1, 0)); // Undirected
    CHECK(hasEdge(g, 0, 3));
    CHECK(countEdges(g) == 4);
    
    g.removeEdge(0, 3);
    CHECK_FALSE(hasEdge(g, 0, 3));
    CHECK_FALSE(hasEdge(g, 3, 0)); // Both directions removed
    CHECK(countEdges(g) == 3);
    
    // Test copy constructor
    graph::Graph g2(g);
    CHECK(g2.getNumVertices() == 4);
    CHECK(hasEdge(g2, 0, 1));
    CHECK(hasEdge(g2, 1, 2));
    CHECK_FALSE(hasEdge(g2, 0, 3));
    
    // Test error cases
    CHECK_THROWS(graph::Graph(-1)); // Negative vertices
    CHECK_THROWS(g.addEdge(0, 4, 1)); // Out of range
    CHECK_THROWS(g.removeEdge(0, 3)); // Edge doesn't exist
}

TEST_CASE("BFS and DFS Trees") {
    graph::Graph g(5);
    g.addEdge(0, 1, 1);
    g.addEdge(0, 2, 1);
    g.addEdge(1, 3, 1);
    g.addEdge(2, 3, 1);
    g.addEdge(2, 4, 1);
    g.addEdge(3, 4, 1);
    
    SUBCASE("BFS Tree") {
        graph::Graph bfsTree = graph::Algorithms::bfs(g, 0);
        
        // BFS tree has n-1 edges
        CHECK(countEdges(bfsTree) == 4);
        
        // Check tree properties
        CHECK(hasEdge(bfsTree, 0, 1));
        CHECK(hasEdge(bfsTree, 0, 2));
        
        // Either 1-3 or 2-3 must exist but not both (tree property)
        bool has1to3 = hasEdge(bfsTree, 1, 3);
        bool has2to3 = hasEdge(bfsTree, 2, 3);
        CHECK((has1to3 || has2to3));
        
        // Tree is connected
        CHECK(isConnected(bfsTree));
    }
    
    SUBCASE("DFS Tree") {
        graph::Graph dfsTree = graph::Algorithms::dfs(g, 0);
        
        // DFS tree has n-1 edges
        CHECK(countEdges(dfsTree) == 4);
        
        // Check tree properties
        CHECK(isConnected(dfsTree));
        
        // Vertices 0, 1, 2, 3, 4 should all be part of the tree
        bool allInTree = true;
        for (int i = 0; i < 5; i++) {
            bool found = false;
            for (int j = 0; j < 5; j++) {
                if (i == j) continue;
                if (hasEdge(dfsTree, i, j)) {
                    found = true;
                    break;
                }
            }
            if (i != 0 && !found) allInTree = false;
        }
        CHECK(allInTree);
    }
}

TEST_CASE("Shortest Paths and MST") {
    graph::Graph g(6);
    // Create a more interesting weighted graph
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 3, 4);
    g.addEdge(2, 4, 5);
    g.addEdge(3, 4, 7);
    g.addEdge(3, 5, 6);
    g.addEdge(4, 5, 8);
    
    SUBCASE("Dijkstra's Algorithm") {
        graph::Graph paths = graph::Algorithms::dijkstra(g, 0);
        
        // Shortest path tree has n-1 edges
        CHECK(countEdges(paths) == 5);
        
        // Tree is connected
        CHECK(isConnected(paths));
        
        // Check that we can reach all vertices
        for (int i = 1; i < 6; i++) {
            bool reachable = false;
            for (int j = 0; j < 6; j++) {
                if (hasEdge(paths, i, j)) {
                    reachable = true;
                    break;
                }
            }
            CHECK(reachable);
        }
    }
    
    SUBCASE("Minimum Spanning Trees") {
        graph::Graph mstPrim = graph::Algorithms::prim(g);
        graph::Graph mstKruskal = graph::Algorithms::kruskal(g);
        
        // Both MSTs have n-1 edges
        CHECK(countEdges(mstPrim) == 5);
        CHECK(countEdges(mstKruskal) == 5);
        
        // Both are connected
        CHECK(isConnected(mstPrim));
        CHECK(isConnected(mstKruskal));
        
        // Both have the same total weight
        CHECK(totalWeight(mstPrim) == totalWeight(mstKruskal));
        
        // Known minimum weight for this graph is 17
        CHECK(totalWeight(mstPrim) == 17);
        
        // Check for some expected edges
        CHECK(hasEdge(mstPrim, 1, 2));  // Weight 1
        CHECK(hasEdge(mstKruskal, 1, 2)); // Weight 1
        CHECK(hasEdge(mstPrim, 1, 3));  // Weight 2
        CHECK(hasEdge(mstKruskal, 1, 3)); // Weight 2
    }
}

TEST_CASE("Special Graph Cases") {
    SUBCASE("Single vertex graph") {
        graph::Graph g(1);
        graph::Graph bfs = graph::Algorithms::bfs(g, 0);
        graph::Graph dfs = graph::Algorithms::dfs(g, 0);
        graph::Graph mst = graph::Algorithms::prim(g);
        
        CHECK(countEdges(bfs) == 0);
        CHECK(countEdges(dfs) == 0);
        CHECK(countEdges(mst) == 0);
    }
    
    SUBCASE("Disconnected graph") {
        graph::Graph g(5);
        g.addEdge(0, 1, 1);
        g.addEdge(2, 3, 2);
        // 4 is isolated
        
        // BFS from vertex 0 should only reach 0 and 1
        graph::Graph bfsTree = graph::Algorithms::bfs(g, 0);
        CHECK(countEdges(bfsTree) == 1);
        CHECK(hasEdge(bfsTree, 0, 1));
        CHECK_FALSE(hasEdge(bfsTree, 0, 2));
        
        // Prim from vertex 0 should still create a minimal spanning forest
        graph::Graph mst = graph::Algorithms::prim(g);
        CHECK(countEdges(mst) == 2); // Two separate trees
        CHECK(hasEdge(mst, 0, 1));
        CHECK(hasEdge(mst, 2, 3));
    }
    
    SUBCASE("Complete graph") {
        graph::Graph g(4);
        for (int i = 0; i < 4; i++) {
            for (int j = i+1; j < 4; j++) {
                g.addEdge(i, j, i+j);
            }
        }
        
        // Should have n(n-1)/2 edges = 6
        CHECK(countEdges(g) == 6);
        
        // MST should have exactly n-1 edges = 3
        graph::Graph mst = graph::Algorithms::kruskal(g);
        CHECK(countEdges(mst) == 3);
        CHECK(isConnected(mst));
    }
}