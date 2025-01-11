// Hassan Khan 2023

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cfloat>

using namespace std;

// edge data structure:
struct EdgeNode {
    // edge node instance variables:
    int index; // edge identifier value
    int u; // source vertex of the edge
    int v; // destination vertex of the edge
    float w; // weight of the edge
    EdgeNode* next; // pointer to the next edge in the list

    // edge node constructor:
    EdgeNode(int inputIndex, int inputU, int inputV, float inputW): 
        index(inputIndex), u(inputU), v(inputV), w(inputW) 
    {}
};

// vertex data structure:
struct VertexNode {
    // vertex node instance variables:
    int index; // vertex identifier value
    bool visited; // flag to mark visited vertices
    float distance; // for Dijkstra's algorithm or other distance-related computations
    int predecessor; // to find predecessor nodes
    EdgeNode* edges; // pointer to the list of edges connected to this vertex
    
    // vertex node constructor:
    VertexNode(int inputIndex, bool inputVisited, float inputDistance, EdgeNode* inputEdges): 
        index(inputIndex), visited(inputVisited), distance(inputDistance), edges(inputEdges) 
    {}
};

// minheap data structure:
class MinHeap {
private:
    // minheap instance variables:
    vector<int> heap;

    // helper function to heapify a subtree rooted at given (input) index:
    void heapify(int index) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < heap.size() && heap[left] < heap[smallest]) {
            smallest = left;
        }

        if (right < heap.size() && heap[right] < heap[smallest]) {
            smallest = right;
        }

        if (smallest != index) {
            swap(heap[index], heap[smallest]);
            heapify(smallest);
        }
    }

public:
    // minheap constructor:
    MinHeap() {}

    // heap insertion method:
    void insert(int value) {
        heap.push_back(value);
        int index = heap.size() - 1;
        while (index > 0 && heap[(index - 1) / 2] > heap[index]) {
        swap(heap[index], heap[(index - 1) / 2]);
            index = (index - 1) / 2;
        }
    }

    // heap minimum element extraction method:
    int extractMin() {
        if (heap.empty()) {
            return -1; // assuming -1 represents an empty heap
        }

        int root = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapify(0);
        return root;
    }

    // heap check if empty method:
    bool isEmpty() const {
        return heap.empty();
    }
};

// stack data structure:
class Stack { // is a stack of vertices
private:
    // stack element (vertex):
    struct Node {
        VertexNode* vertex;
        Node* next;

        // stack element constructor:
        Node(VertexNode* v): 
            vertex(v), next(nullptr) 
        {}
    };

    Node* topNode; // node to keep track of top element

public:
    // stack constructor:
    Stack(): 
        topNode(nullptr)
    {}

    // stack deconstructor:
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // stack check if empty method:
    bool isEmpty() const {
        return topNode == nullptr;
    }

    // stack push method:
    void push(VertexNode* v) {
        Node* newNode = new Node(v);
        newNode->next = topNode;
        topNode = newNode;
    }

    // stack pop method:
    VertexNode* pop() {
        if (isEmpty()) {
            cout << "Stack is empty. Cannot pop from an empty stack.\n";
            return nullptr; // return NULL if stack is empty
        }
    
        Node* temp = topNode; // set a temp for the contents of the top node
        topNode = topNode->next; // make the new top node of the stack the node under the previous top node

        VertexNode* poppedVertex = temp->vertex; // save the popped node
        delete temp; // delete the popped node from the stack

        return poppedVertex; // return the popped element
    }
};

// graph class:
class Graph {
public:
    vector<VertexNode*> vertices; // array of pointers to vertex nodes

    // graph constructor to initialize an empty graph:
    Graph(int numVertices) {
        vertices.resize(numVertices + 1); // assuming vertices are indexed from 1 to "numVertices"

        for (int i = 1; i <= numVertices; ++i) {
            vertices[i] = new VertexNode(i, false, 0.0, nullptr); // initializing with proper values
        }
    }

    // graph add edge method:
    void addEdge(int index, int u, int v, float w, int directedFlag, int flagNumber) {
        EdgeNode* newEdge1 = new EdgeNode(index, u, v, w);

        if (flagNumber == 1) { // case if user enter's flag value as 1
            newEdge1->next = vertices[u]->edges;
            vertices[u]->edges = newEdge1;
        } // nodes are inserted at the front of the adjacency list
        else if (flagNumber == 2) { // case if user enter's flag value as 2
            EdgeNode* temp = vertices[u]->edges;
            while (temp != nullptr && temp->next != nullptr) {
                temp = temp->next;
            }
            if (temp == nullptr) {
                vertices[u]->edges = newEdge1;
            } 
            else {
                temp->next = newEdge1;
            }
        } // nodes are inserted at the back of the adjacency list

        if (directedFlag == 2) { // case if user enter's graph as "UD"
            EdgeNode* currentEdgeU = vertices[u]->edges;
            EdgeNode* currentEdgeV = vertices[v]->edges;
            bool existsU = false;
            bool existsV = false;
            // variables to check if edges already exist between the vertices "u" and "v" in both directions

            // searches for edges in both directions:
            while (currentEdgeU != nullptr || currentEdgeV != nullptr) {
                if (currentEdgeU != nullptr && currentEdgeU->v == v) {
                    existsU = true;
                }
                if (currentEdgeV != nullptr && currentEdgeV->v == u) {
                    existsV = true;
                }
                if (existsU && existsV) {
                    return;
                }
                if (currentEdgeU != nullptr) {
                    currentEdgeU = currentEdgeU->next;
                }
                if (currentEdgeV != nullptr) {
                    currentEdgeV = currentEdgeV->next;
                }
            }

            // if edge "u -> v" doesn't exist, add it in the reverse direction "v -> u"
            if (!existsU) {
                EdgeNode* newEdge2 = new EdgeNode(index, v, u, w);
                if (flagNumber == 1) {
                    newEdge2->next = vertices[v]->edges;
                    vertices[v]->edges = newEdge2;
                } else if (flagNumber == 2) {
                    EdgeNode* temp = vertices[v]->edges;
                    while (temp != nullptr && temp->next != nullptr) {
                        temp = temp->next;
                    }

                    if (temp == nullptr) {
                        vertices[v]->edges = newEdge2;
                    } else {
                        temp->next = newEdge2;
                    }
                }
            }
        }
    }

    // graph adjacency list printing method:
    void printAdjList() const {
        for (size_t i = 1; i < vertices.size(); ++i) {
            
            cout << "ADJ[" << i << "]:-->";
            EdgeNode* currentEdge = vertices[i]->edges;

            if (currentEdge != nullptr) {
                while (currentEdge->next != nullptr) {
                    cout << "[" << currentEdge->u << " " << currentEdge->v << ": " << fixed << setprecision(2) << currentEdge->w << "]-->";
                    currentEdge = currentEdge->next;
                }
                cout << "[" << currentEdge->u << " " << currentEdge->v << ": " << fixed << setprecision(2) << currentEdge->w << "]";
            } 
            else {
                cout << "NULL";
            } // cases to handle when or when not to put "-->" or "NULL" after a node
            
            cout << endl;
        }
    }

    // graph print path method for case '5':
    void printPath(Graph& graph, int source, int destination) {
        if (source == destination) {
            cout << "[" << source << ": 0.00]" << endl;
            return;
        }

        Stack stack;
        int parent = graph.vertices[destination]->predecessor;

        stack.push(graph.vertices[destination]);

        while (source != parent && parent != -1) {
            stack.push(graph.vertices[parent]);
            parent = graph.vertices[parent]->predecessor;
        }

        cout << "[" << source << ": 0.00]";

        while (!stack.isEmpty()) {
            VertexNode* top = stack.pop();
            cout << "-->[" << top->index << ": " << fixed << setprecision(2) << top->distance << "]";
        }

        cout << endl;
    }
};

//-------------------------------------------------------------------------------------------

// Dijkstra's algorithm method:
void Dijkstra(vector<VertexNode*>& vertices, vector<EdgeNode*>& adjacencyList, int source) {
    MinHeap pq; // pq = "priority queue"
    pq.insert(source);
    vertices[source]->distance = 0;

    // keeps track of which vertices are in the priority queue
    vector<bool> inQueue(vertices.size(), false);
    inQueue[source] = true;

    while (!pq.isEmpty()) {
        int u = pq.extractMin();
        inQueue[u] = false; // marks vertex as processed
        EdgeNode* edge = vertices[u]->edges;

        while (edge != nullptr) {
            int v = edge->v;
            float weight = edge->w;

            if (vertices[v]->distance > vertices[u]->distance + weight) {
                vertices[v]->distance = vertices[u]->distance + weight;
                vertices[v]->predecessor = u; // updates predecessor here

                if (!inQueue[v]) {
                    pq.insert(v);
                    inQueue[v] = true;
                }
            }

            edge = edge->next;
        }
    }
}

//-------------------------------------------------------------------------------------------

// main method:
int main() {
    // main method instance variables:
    int lastSource = -1;
    int lastDestination = -1; // cases to keep track of most recent path computation
    int n, m; // n = # of vertices, m = # of edges
    vector<EdgeNode*> adjacencyList; // stores the adjacency list for Dijkstra's algorithm
    char userMenuInput; // variable to store the value of the user's main menu inputs
    string fileName; // stores the file name
    int directedFlag; // determines whether the file is directed or not
    bool singleSourceComputed = false; // value to keep track on whether single source or pair was recently executed

    Graph graph(0); // Creating the graph object to be used

    do {
        printf("\n");
        printf("1. Read the graph from the file (enter the file name, D or UD and the flag)\n");
        printf("for example network01.txt D 1 or 2 for the directed graph\n");
        printf("2. Print ADJ\n");
        printf("3. Single Source, enter the starting node number\n");
        printf("4. Single Pair, enter the starting node and ending node numbers\n");
        printf("5. Print Path, enter the starting node and ending node numbers\n");
        printf("6. Print Length, enter the starting node and ending node numbers\n");
        printf("7. Stop\n");
        printf("\n");
        printf("Enter options 1 - 7\n"); // menu displayed to user

        cin >> userMenuInput; // user input is copied to variable "userinput"

        switch(userMenuInput) {
            case '1': { // read graph from file:
                cout << "Enter the file name: ";
                cin >> fileName;
                ifstream inputFile(fileName);

                if (!inputFile.is_open()) {
                    cout << "Failed to open the file!" << endl;
                    break;
                }

                inputFile >> n >> m; // # of verticies and edges are stored
                graph = Graph(n); // create a graph with "n" vertices

                cout << "Enter \"D\" for directed graph or \"UD\" for undirected graph: ";
                string directedStr; // used to convert the string into an int
                cin >> directedStr;

                directedFlag = (directedStr == "D" || directedStr == "d") ? 1 : 2;
                // if the user enters "D" or "d", directedFlag = 1, else = 2

                cout << "Enter \"1\" for adding nodes to the front of the adjacency list or \"2\" for adding nodes to the back: ";
                int flagNumber;
                cin >> flagNumber; // used to store the flag value for node insertion order

                int edgeIndex, u, v;
                float w; // used to read subsequent lines in the file
                for (int i = 0; i < m; ++i) {
                    inputFile >> edgeIndex >> u >> v >> w;
                    graph.addEdge(edgeIndex, u, v, w, directedFlag, flagNumber);

                    if (directedFlag == 2) {
                        graph.addEdge(edgeIndex, v, u, w, directedFlag, flagNumber);
                    }
                } // adding edges in a specific way depending on if the graph is directed or not

                inputFile.close(); // close file
                cout << "Graph read from file." << endl;
                break; // case '1' end
            }

            case '2': { // print adjacency list:
                graph.printAdjList(); // calls print adjacency list method
                break; // case '2' end
            }

            case '3': { // single source:
                lastSource = -1;
                lastDestination = -1; // reset values to check most recent path computation
                int source; // value to store source vertex

                cout << "Enter Source Node: ";
                cin >> source; // accepting source node value from user

                if (source < 1 || source > n) {
                    cout << "Invalid source node." << endl;
                    break;
                } // validates source and destination nodes within the range

                for (int i = 1; i <= n; ++i) {
                    graph.vertices[i]->distance = FLT_MAX; // Initialize distances to FLT_MAX
                    graph.vertices[i]->predecessor = -1;   // Initialize predecessors to -1 (nil)
                } // resets the vertices for a new computation

                Dijkstra(graph.vertices, adjacencyList, source);
                // executes Dijkstra's algorithm

                cout << "Shortest Path computation from " << source << " completed." << endl;
                lastSource = source; 
                singleSourceComputed = true;
                // assigns values to let system know most recent path computation was single source
                break; // case '3' end
            }

            case '4': { // single pair:
                lastSource = -1;
                lastDestination = -1; // reset values to check most recent path computation
                int source, destination; // value to store source and destination vertices

                cout << "Enter Source Node: ";
                cin >> source;
                cout << "Enter Destination Node: ";
                cin >> destination; // accepting source and destination node value from user

                if (source < 1 || source > n || destination < 1 || destination > n) {
                    cout << "Invalid source or destination node." << endl;
                    break;
                } // validates source and destination nodes within the range

                for (int i = 1; i <= n; ++i) {
                    graph.vertices[i]->distance = FLT_MAX; // Initialize distances to FLT_MAX
                    graph.vertices[i]->predecessor = -1;   // Initialize predecessors to -1 (nil)
                } // resets the vertices for a new computation

                Dijkstra(graph.vertices, adjacencyList, source);
                // executes Dijkstra's algorithm

                cout << "Shortest Path computation from " << source << " to " << destination << " completed." << endl;
                lastSource = source;
                lastDestination = destination;
                singleSourceComputed = false;
                // assigns values to let system know most recent path computation was single pair
                break; // case '4' end
            }

            case '5': { // print path:
                int source, destination;
                cout << "Enter Source Node: ";
                cin >> source;
                cout << "Enter Destination Node: ";
                cin >> destination; // accepting source and destination node value from user

                if (((!singleSourceComputed) && (source != lastSource)) || ((source != lastSource) && (destination != lastDestination))) {
                    cout << "There is no path from " << source << " to " << destination << "." << endl;
                } // checks if the destination is reachable from the source (based on values set previously)
                // first condition is to permit single source and second is to permit single pair
                else {
                    if (graph.vertices[destination]->distance == FLT_MAX) {
                        cout << "There is no path from " << source << " to " << destination << "." << endl;
                    } // checks if the destination is reachable from the source
                    else {
                        cout << "Shortest Path from " << source << " to " << destination << " is: " << endl;
                        cout << "Path: ";
                        graph.printPath(graph, source, destination);
                        cout << endl;
                    } // prints the path
                }
                break; // case '5' end
            }

            case '6': { // print length:
                int source, destination;
                cout << "Enter Source Node: ";
                cin >> source;
                cout << "Enter Destination Node: ";
                cin >> destination; // accepting source and destination node value from user

                // Check if the entered source and destination match the most recent path computation
                if (((!singleSourceComputed) && (source != lastSource)) || ((source != lastSource) && (destination != lastDestination))) {
                    cout << "There is no path from " << source << " to " << destination << "." << endl;
                } // checks if the destination is reachable from the source (based on values set previously)
                // first condition is to permit single source and second is to permit single pair
                else {
                    if (graph.vertices[destination]->distance == FLT_MAX) {
                        cout << "There is no path from " << source << " to " << destination << "." << endl;
                    } // checks if the destination is reachable from the source
                    else {
                        cout << "Distance from " << source << " to " << destination << " is: " << graph.vertices[destination]->distance << endl;
                    } // prints the distance
                }
                break; // case '6' end
            }

            case '7': { // stop program:
                break; // case '7' end
            }

            default: { // if user enters input besides '1', '2', '3', ..., '7':
                printf("Invalid input. Please enter a valid option.\n");
                break; // default end
            }
        }
    } while (userMenuInput != '7');

    return 0;
}