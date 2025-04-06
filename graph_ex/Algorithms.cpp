#ifndef ALGORITHMS_CPP
#define ALGORITHMS_CPP

#include "Graph.cpp"

namespace graph {

// --- Queue Implementation ---
class Queue {
private:
    struct QNode {
        int data;
        QNode* next;
        QNode(int val) : data(val), next(nullptr) {}
    };
    QNode* head; // front
    QNode* tail; // rear
public:
    Queue() : head(nullptr), tail(nullptr) {}
    ~Queue() {
        while (head != nullptr) {
            QNode* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }
    void enqueue(int value) {
        QNode* newNode = new QNode(value);
        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    int dequeue() {
        if (head == nullptr)
            throw "Queue is empty";
        int val = head->data;
        QNode* oldHead = head;
        head = head->next;
        if (head == nullptr)
            tail = nullptr;
        delete oldHead;
        return val;
    }
    bool isEmpty() const {
        return head == nullptr;
    }
};

// --- PriorityQueue Implementation ---
class PriorityQueue {
private:
    struct PQNode {
        int vertex;
        int priority;
        PQNode() : vertex(-1), priority(0) {}
        PQNode(int v, int p) : vertex(v), priority(p) {}
    };
    PQNode* arr;
    int* pos; // maps vertex to its position in arr
    int capacity;
    int size;
    void heapifyDown(int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < size && arr[left].priority < arr[smallest].priority)
            smallest = left;
        if (right < size && arr[right].priority < arr[smallest].priority)
            smallest = right;
        if (smallest != i) {
            pos[arr[i].vertex] = smallest;
            pos[arr[smallest].vertex] = i;
            PQNode temp = arr[i];
            arr[i] = arr[smallest];
            arr[smallest] = temp;
            heapifyDown(smallest);
        }
    }
    void heapifyUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (arr[parent].priority > arr[i].priority) {
                pos[arr[parent].vertex] = i;
                pos[arr[i].vertex] = parent;
                PQNode temp = arr[parent];
                arr[parent] = arr[i];
                arr[i] = temp;
                i = parent;
            } else {
                break;
            }
        }
    }
public:
    PriorityQueue(int cap) : capacity(cap), size(0) {
        arr = new PQNode[capacity];
        pos = new int[capacity];
        for (int i = 0; i < capacity; i++)
            pos[i] = -1;
    }
    ~PriorityQueue() {
        delete[] arr;
        delete[] pos;
    }
    bool isEmpty() const {
        return size == 0;
    }
    bool inQueue(int vertex) const {
        return pos[vertex] != -1;
    }
    void insert(int vertex, int prio) {
        if (size == capacity)
            throw "Priority queue is full";
        arr[size] = PQNode(vertex, prio);
        pos[vertex] = size;
        size++;
        heapifyUp(size - 1);
    }
    int extractMin() {
        if (size == 0)
            throw "Priority queue is empty";
        int minVertex = arr[0].vertex;
        pos[minVertex] = -1;
        arr[0] = arr[size - 1];
        pos[arr[0].vertex] = 0;
        size--;
        heapifyDown(0);
        return minVertex;
    }
    void decreaseKey(int vertex, int newPrio) {
        int i = pos[vertex];
        if (i == -1)
            throw "Vertex not in priority queue";
        if (newPrio > arr[i].priority)
            throw "New priority is greater than current priority";
        arr[i].priority = newPrio;
        heapifyUp(i);
    }
    int getPriority(int vertex) const {
        int i = pos[vertex];
        if (i == -1)
            throw "Vertex not in priority queue";
        return arr[i].priority;
    }
};

// --- UnionFind Implementation ---
class UnionFind {
private:
    int* parent;
    int* rank;
    int n;
public:
    UnionFind(int n) : n(n) {
        parent = new int[n];
        rank = new int[n];
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            rank[i] = 0;
        }
    }
    ~UnionFind() {
        delete[] parent;
        delete[] rank;
    }
    // Iterative path compression (using path halving)
    int find(int x) {
        if (x < 0 || x >= n)
            throw "Index out of range";
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }
    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY)
            return;
        if (rank[rootX] < rank[rootY])
            parent[rootX] = rootY;
        else if (rank[rootX] > rank[rootY])
            parent[rootY] = rootX;
        else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
    }
    bool isConnected(int x, int y) {
        return find(x) == find(y);
    }
};

// --- Algorithms Class ---
class Algorithms {
public:
    // BFS algorithm - returns a BFS tree graph from traversal
    static Graph bfs(const Graph& g, int source);
    // DFS algorithm - returns a DFS tree/forest from traversal
    static Graph dfs(const Graph& g, int source);
    // Dijkstra's algorithm - returns a weighted tree of shortest paths
    static Graph dijkstra(const Graph& g, int source);
    // Prim's algorithm - returns a minimum spanning tree
    static Graph prim(const Graph& g);
    // Kruskal's algorithm - returns a minimum spanning tree
    static Graph kruskal(const Graph& g);
private:
    // (No recursive DFS helper is used in this version.)
};

const int MAX_INT_VALUE = 2147483647;

// --- BFS Implementation ---
Graph Algorithms::bfs(const Graph& g, int source) {
    int n = g.getNumVertices();
    if (source < 0 || source >= n)
        throw "Source vertex out of range";
    Graph result(n);
    bool* visited = new bool[n](); // All false
    Queue q;
    visited[source] = true;
    q.enqueue(source);
    // Use a lambda to encapsulate the processing loop.
    auto processQueue = [&]() {
        while (!q.isEmpty()) {
            int cur = q.dequeue();
            for (Graph::Edge* e = g.getAdjList(cur); e != nullptr; e = e->next) {
                int nb = e->destination;
                if (!visited[nb]) {
                    visited[nb] = true;
                    q.enqueue(nb);
                    result.addEdge(cur, nb, e->weight);
                }
            }
        }
    };
    processQueue();
    delete[] visited;
    return result;
}

// --- DFS Implementation (Iterative) ---
Graph Algorithms::dfs(const Graph& g, int source) {
    int n = g.getNumVertices();
    if (source < 0 || source >= n)
        throw "Source vertex out of range";
    Graph result(n);
    bool* visited = new bool[n](); // All false
    // Define a stack item that holds vertex, parent, and the weight of the edge from parent.
    struct StackItem {
        int vertex;
        int parent;
        int weight;
    };
    StackItem* stack = new StackItem[n]; // Maximum n items
    int top = 0;
    stack[top] = {source, -1, 0};
    while (top >= 0) {
        StackItem curItem = stack[top--];
        if (!visited[curItem.vertex]) {
            visited[curItem.vertex] = true;
            if (curItem.parent != -1)
                result.addEdge(curItem.parent, curItem.vertex, curItem.weight);
            // Push neighbors onto the stack.
            for (Graph::Edge* e = g.getAdjList(curItem.vertex); e != nullptr; e = e->next) {
                if (!visited[e->destination]) {
                    stack[++top] = {e->destination, curItem.vertex, e->weight};
                }
            }
        }
    }
    delete[] stack;
    delete[] visited;
    return result;
}

// --- Dijkstra's Algorithm Implementation ---
Graph Algorithms::dijkstra(const Graph& g, int source) {
    int n = g.getNumVertices();
    if (source < 0 || source >= n)
        throw "Source vertex out of range";
    Graph result(n);
    int* dist = new int[n];
    int* par = new int[n];
    int* edgeW = new int[n];
    for (int i = 0; i < n; i++) {
        dist[i] = MAX_INT_VALUE;
        par[i] = -1;
        edgeW[i] = 0;
    }
    dist[source] = 0;
    PriorityQueue pq(n);
    for (int i = 0; i < n; i++) {
        pq.insert(i, dist[i]);
    }
    while (!pq.isEmpty()) {
        int u = pq.extractMin();
        for (Graph::Edge* e = g.getAdjList(u); e != nullptr; e = e->next) {
            int v = e->destination;
            if (pq.inQueue(v) && dist[u] != MAX_INT_VALUE && (dist[u] + e->weight < dist[v])) {
                dist[v] = dist[u] + e->weight;
                par[v] = u;
                edgeW[v] = e->weight;
                pq.decreaseKey(v, dist[v]);
            }
        }
    }
    // Build the shortest-path tree from the parent pointers.
    for (int i = 0; i < n; i++) {
        if (i != source && par[i] != -1)
            result.addEdge(par[i], i, edgeW[i]);
    }
    delete[] dist;
    delete[] par;
    delete[] edgeW;
    return result;
}

// --- Prim's Algorithm Implementation ---
Graph Algorithms::prim(const Graph& g) {
    int n = g.getNumVertices();
    Graph result(n);
    int* key = new int[n];
    int* par = new int[n];
    for (int i = 0; i < n; i++) {
        key[i] = MAX_INT_VALUE;
        par[i] = -1;
    }
    key[0] = 0;
    PriorityQueue pq(n);
    for (int i = 0; i < n; i++) {
        pq.insert(i, key[i]);
    }
    while (!pq.isEmpty()) {
        int u = pq.extractMin();
        for (Graph::Edge* e = g.getAdjList(u); e != nullptr; e = e->next) {
            int v = e->destination;
            if (pq.inQueue(v) && e->weight < key[v]) {
                key[v] = e->weight;
                par[v] = u;
                pq.decreaseKey(v, key[v]);
            }
        }
    }
    // Build the MST from the parent pointers (vertex 0 is the starting point).
    for (int i = 1; i < n; i++) {
        if (par[i] != -1)
            result.addEdge(par[i], i, key[i]);
    }
    delete[] key;
    delete[] par;
    return result;
}

// --- Kruskal's Algorithm Implementation ---
Graph Algorithms::kruskal(const Graph& g) {
    int n = g.getNumVertices();
    Graph result(n);
    // Define a local structure for edge information.
    struct EdgeInfo {
        int u, v, weight;
    };
    int maxEdges = n * (n - 1) / 2;
    EdgeInfo* edges = new EdgeInfo[maxEdges];
    int count = 0;
    // Collect edges, avoiding duplicates in the undirected graph.
    for (int i = 0; i < n; i++) {
        for (Graph::Edge* e = g.getAdjList(i); e != nullptr; e = e->next) {
            if (i < e->destination)
                edges[count++] = {i, e->destination, e->weight};
        }
    }
    // Sort edges by weight using selection sort.
    for (int i = 0; i < count - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < count; j++) {
            if (edges[j].weight < edges[minIndex].weight)
                minIndex = j;
        }
        if (minIndex != i) {
            EdgeInfo temp = edges[i];
            edges[i] = edges[minIndex];
            edges[minIndex] = temp;
        }
    }
    // Build the MST using the Union-Find structure.
    UnionFind uf(n);
    for (int i = 0; i < count; i++) {
        if (!uf.isConnected(edges[i].u, edges[i].v)) {
            result.addEdge(edges[i].u, edges[i].v, edges[i].weight);
            uf.unite(edges[i].u, edges[i].v);
        }
    }
    delete[] edges;
    return result;
}

} // namespace graph

#endif // ALGORITHMS_CPP
