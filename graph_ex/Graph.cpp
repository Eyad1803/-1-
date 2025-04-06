// graph.cpp
#ifndef GRAPH_CPP
#define GRAPH_CPP

#include <iostream>

namespace graph {

class Graph {
public:
    // This struct represents an edge in the adjacency list.
    struct Edge {
        int destination;
        int weight;
        Edge* next;
        
        Edge(int dest, int w) : destination(dest), weight(w), next(0) {}
        // Destructor is empty; cleanup is handled by Graph.
        ~Edge() {}
    };

    // Constructors, destructor, and assignment operator.
    Graph(int vertices);
    ~Graph();
    Graph(const Graph& other);
    Graph& operator=(Graph other); // Uses copy-swap idiom.
    friend void swap(Graph& a, Graph& b);

    // Graph operations.
    void addEdge(int source, int dest, int weight = 1);
    void removeEdge(int source, int dest);
    void print_graph() const;

    // Accessors.
    int getNumVertices() const;
    Edge* getAdjList(int vertex) const;

private:
    Edge** adjacencyList; // Array of pointers to linked lists.
    int numVertices;
    
    // Helper functions.
    Edge* deepCopyEdgeList(Edge* head) const; // Iterative deep copy.
    void freeEdgeList(Edge* head);            // Iterative deletion.
};

// Constructor: allocate the array for adjacency lists.
Graph::Graph(int vertices) : numVertices(vertices) {
    if (vertices <= 0)
        throw "Number of vertices must be positive";
    adjacencyList = new Edge*[numVertices];
    for (int i = 0; i < numVertices; ++i)
        adjacencyList[i] = 0;
}

// Destructor: free each edge list.
Graph::~Graph() {
    for (int i = 0; i < numVertices; ++i)
        freeEdgeList(adjacencyList[i]);
    delete [] adjacencyList;
}

// Helper: iteratively free a linked list of edges.
void Graph::freeEdgeList(Edge* head) {
    while (head) {
        Edge* temp = head;
        head = head->next;
        delete temp;
    }
}

// Iterative deep-copy of an edge list.
Graph::Edge* Graph::deepCopyEdgeList(Edge* head) const {
    if (!head)
        return 0;
    Edge* newHead = new Edge(head->destination, head->weight);
    Edge* copyTail = newHead;
    head = head->next;
    while (head) {
        copyTail->next = new Edge(head->destination, head->weight);
        copyTail = copyTail->next;
        head = head->next;
    }
    return newHead;
}

// Copy constructor.
Graph::Graph(const Graph& other) : numVertices(other.numVertices) {
    adjacencyList = new Edge*[numVertices];
    for (int i = 0; i < numVertices; ++i)
        adjacencyList[i] = deepCopyEdgeList(other.adjacencyList[i]);
}

// Local swap function (using fully-qualified Edge type).
void swap(Graph& a, Graph& b) {
    int tempNum = a.numVertices;
    a.numVertices = b.numVertices;
    b.numVertices = tempNum;
    Graph::Edge** tempList = a.adjacencyList;
    a.adjacencyList = b.adjacencyList;
    b.adjacencyList = tempList;
}

// Assignment operator using copy-swap.
Graph& Graph::operator=(Graph other) {
    swap(*this, other);
    return *this;
}

// addEdge: Appends a new undirected edge by adding at the tail.
void Graph::addEdge(int source, int dest, int weight) {
    if (source < 0 || source >= numVertices || dest < 0 || dest >= numVertices)
        throw "Vertex index out of range";
    
    // Append new edge from source to dest.
    Edge* newEdge = new Edge(dest, weight);
    if (!adjacencyList[source])
        adjacencyList[source] = newEdge;
    else {
        Edge* curr = adjacencyList[source];
        while (curr->next)
            curr = curr->next;
        curr->next = newEdge;
    }
    
    // For undirected graph, add the reverse edge.
    Edge* reverseEdge = new Edge(source, weight);
    if (!adjacencyList[dest])
        adjacencyList[dest] = reverseEdge;
    else {
        Edge* curr = adjacencyList[dest];
        while (curr->next)
            curr = curr->next;
        curr->next = reverseEdge;
    }
}

// removeEdge: Remove the edge from source->dest and dest->source.
// Note: We call the helper for both directions independently.
void Graph::removeEdge(int source, int dest) {
    if (source < 0 || source >= numVertices || dest < 0 || dest >= numVertices)
        throw "Vertex index out of range";
    
    bool removedForward = false;
    bool removedReverse = false;
    
    // Lambda to remove a single directed edge.
    auto removeSingle = [this](int from, int to) -> bool {
        Edge** currPtr = &adjacencyList[from];
        while (*currPtr) {
            if ((*currPtr)->destination == to) {
                Edge* temp = *currPtr;
                *currPtr = (*currPtr)->next;
                temp->next = 0;
                delete temp;
                return true;
            }
            currPtr = &((*currPtr)->next);
        }
        return false;
    };
    
    removedForward = removeSingle(source, dest);
    removedReverse = removeSingle(dest, source);
    
    if (!removedForward && !removedReverse)
        throw "Edge does not exist";
}

// print_graph: Prints the adjacency list of each vertex.
void Graph::print_graph() const {
    for (int i = 0; i < numVertices; ++i) {
        std::cout << "Vertex " << i << " -> ";
        for (Edge* curr = adjacencyList[i]; curr != 0; curr = curr->next)
            std::cout << "(" << curr->destination << ", weight: " << curr->weight << ") ";
        std::cout << "\n";
    }
}

// getNumVertices: Returns the number of vertices.
int Graph::getNumVertices() const {
    return numVertices;
}

// getAdjList: Returns the adjacency list for a vertex.
Graph::Edge* Graph::getAdjList(int vertex) const {
    if (vertex < 0 || vertex >= numVertices)
        throw "Vertex index out of range";
    return adjacencyList[vertex];
}

} // namespace graph

#endif // GRAPH_CPP
