# Graph Algorithms Library

A robust C++ implementation of common graph data structures and algorithms. This library provides an efficient undirected graph implementation along with implementations of classic graph algorithms.

## Features

- **Graph Data Structure**:
  - Efficient adjacency list representation
  - Support for weighted, undirected graphs
  - Copy constructor and assignment operator
  - Memory leak-free implementation

- **Graph Algorithms**:
  - **Traversal Algorithms**:
    - Breadth-First Search (BFS)
    - Depth-First Search (DFS)
  - **Shortest Path Algorithms**:
    - Dijkstra's Algorithm
  - **Minimum Spanning Tree Algorithms**:
    - Prim's Algorithm
    - Kruskal's Algorithm

- **Data Structures**:
  - Queue (custom implementation)
  - Priority Queue (custom min-heap implementation)
  - Union-Find/Disjoint Set (for Kruskal's algorithm)

## Project Structure

- `Graph.cpp` - Core graph data structure implementation
- `Algorithms.cpp` - Implementation of graph algorithms
- `main.cpp` - Demonstration of graph operations
- `tests.cpp` - Comprehensive test suite using doctest
- `doctest.h` - Header-only testing framework
- `Makefile` - Build system

## API Reference

### Graph.cpp

The `Graph` class provides the foundation for storing and manipulating graphs.

```cpp
namespace graph {
    class Graph {
    public:
        // Constructors and destructor
        Graph(int vertices);                   // Create a graph with 'vertices' number of vertices
        Graph(const Graph& other);             // Copy constructor
        ~Graph();                              // Destructor
        
        // Operators
        Graph& operator=(const Graph& other);  // Assignment operator
        
        // Graph operations
        void addEdge(int source, int dest, int weight = 1);  // Add an edge between two vertices
        void removeEdge(int source, int dest); // Remove an edge
        void print_graph() const;              // Display the graph
        
        // Accessors
        int getNumVertices() const;            // Get the number of vertices
        Edge* getAdjList(int vertex) const;    // Get adjacency list for a vertex
    };
}
```

### Algorithms.cpp

The `Algorithms` class contains static methods for various graph algorithms.

```cpp
namespace graph {
    class Algorithms {
    public:
        // Traversal algorithms
        static Graph bfs(const Graph& g, int source);  // Breadth-First Search from source
        static Graph dfs(const Graph& g, int source);  // Depth-First Search from source
        
        // Shortest path algorithm
        static Graph dijkstra(const Graph& g, int source); // Dijkstra's algorithm from source
        
        // Minimum spanning tree algorithms
        static Graph prim(const Graph& g);     // Prim's MST algorithm
        static Graph kruskal(const Graph& g);  // Kruskal's MST algorithm
    };
    
    // Helper data structures (used internally)
    class Queue {
    public:
        Queue();                      // Constructor
        ~Queue();                     // Destructor
        void enqueue(int value);      // Add element to queue
        int dequeue();                // Remove and return front element
        bool isEmpty() const;         // Check if queue is empty
    };
    
    class PriorityQueue {
    public:
        PriorityQueue(int capacity);  // Constructor with capacity
        ~PriorityQueue();             // Destructor
        bool isEmpty() const;         // Check if queue is empty
        bool inQueue(int vertex) const; // Check if vertex is in the queue
        void insert(int vertex, int priority); // Insert with priority
        int extractMin();             // Extract minimum priority element
        void decreaseKey(int vertex, int newPriority); // Decrease priority
        int getPriority(int vertex) const; // Get priority of vertex
    };
    
    class UnionFind {
    public:
        UnionFind(int n);             // Constructor for n elements
        ~UnionFind();                 // Destructor
        int find(int x);              // Find representative of x's set
        void unite(int x, int y);     // Unite sets containing x and y
        bool isConnected(int x, int y); // Check if x and y are in same set
    };
}
```

### main.cpp

The `main.cpp` file demonstrates the usage of the Graph and Algorithms classes:

```cpp
int main() {
    // Create a graph with 6 vertices
    graph::Graph g(6);
    
    // Add edges with weights
    g.addEdge(0, 1, 4);
    ...
    
    // Print the graph
    g.print_graph();
    
    // BFS traversal from vertex 0
    graph::Graph bfsTree = graph::Algorithms::bfs(g, 0);
    
    // DFS traversal from vertex 0
    graph::Graph dfsTree = graph::Algorithms::dfs(g, 0);
    
    // Dijkstra's shortest paths from vertex 0
    graph::Graph shortestPaths = graph::Algorithms::dijkstra(g, 0);
    
    // Prim's MST
    graph::Graph primMST = graph::Algorithms::prim(g);
    
    // Kruskal's MST
    graph::Graph kruskalMST = graph::Algorithms::kruskal(g);
    
    return 0;
}
```

### tests.cpp

The `tests.cpp` file contains comprehensive tests using the doctest framework:

```cpp
// Helper functions available for testing
int countEdges(const graph::Graph& g);              // Count unique edges
int totalWeight(const graph::Graph& g);             // Sum weights of all edges
bool hasEdge(const graph::Graph& g, int src, int dest); // Check if edge exists
bool isConnected(const graph::Graph& g);            // Check if graph is connected

// Test cases
TEST_CASE("Basic Graph Operations");                // Basic graph functionality
TEST_CASE("BFS and DFS Trees");                     // Traversal algorithms
TEST_CASE("Shortest Paths and MST");                // Path and tree algorithms
TEST_CASE("Special Graph Cases");                   // Edge cases
```

## Usage Example

```cpp
#include "algorithms.cpp"
#include <iostream>

int main() {
    // Create a graph with 6 vertices
    graph::Graph g(6);
    
    // Add edges with weights
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 5);
    // ... more edges ...
    
    // Print the graph
    g.print_graph();
    
    // Find shortest paths from vertex 0
    graph::Graph shortestPaths = graph::Algorithms::dijkstra(g, 0);
    
    // Find minimum spanning tree
    graph::Graph mst = graph::Algorithms::kruskal(g);
    
    return 0;
}
```

## Building and Running

### Prerequisites

- C++11 compatible compiler (g++ recommended)
- Make (for building with the Makefile)

### Commands

```bash
# Build and run the main program
make Main

# Build and run the tests
make test

# Build all targets
make all

# Memory leak check using Valgrind
make valgrind
make valgrind-test

# Clean build artifacts
make clean
```

## Implementation Details

### Graph Data Structure

The graph is implemented using an adjacency list representation, where each vertex maintains a linked list of its adjacent vertices. The implementation supports weighted, undirected graphs.

### BFS and DFS

The BFS implementation uses a queue for traversal, while the DFS implementation uses an iterative approach with a stack instead of recursion to avoid stack overflow for large graphs.

### Dijkstra's Algorithm

The implementation uses a priority queue (binary min-heap) to efficiently find the vertex with the minimum distance at each step, resulting in an O((V+E)log V) time complexity.

### Minimum Spanning Tree Algorithms

- **Prim's Algorithm**: Uses a priority queue to select the minimum-weight edge at each step.
- **Kruskal's Algorithm**: Uses a Union-Find data structure to detect cycles while constructing the MST.

## Testing

The project includes a comprehensive test suite implemented using the doctest framework. Tests cover:

- Basic graph operations
- Graph traversal algorithms
- Shortest path algorithms
- Minimum spanning tree algorithms
- Special cases (single vertex, disconnected graphs, complete graphs)

Run the tests with:

```bash
make test
```

## Memory Management

The implementation carefully manages memory to prevent leaks. The destructor properly cleans up all dynamically allocated resources. Memory leak checking can be performed using:

```bash
make valgrind
```

## License

[Add your license information here]
