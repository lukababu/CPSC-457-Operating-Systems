/************************************************************************************
 * Last Name:   Iremadze
 * First Name:  Luke
 * Student ID:  10163614
 * Course:      CPSC 457
 * Tutorial:    T04
 * Assignment:  4
 * Question:    Q4
 * File name:	deadlock.cpp
 *
 * Compile:	gcc scheduler.c -o scheduler
 * 
 * Execute:
 *  Option 1:
 *      $./scheduler
 *  Option 2:
 *      $./scheduler < config.txt
 *
*************************************************************************************/
// Cormen, Thomas, Charles Leiserson, Ronald Rivest, and Clifford Stein. Introduction to Algorithms. 3rd ed. MIT Press, 2009. ISBN: 9780262033848.
// https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-006-introduction-to-algorithms-fall-2011/lecture-videos/MIT6_006F11_lec14.pdf
// https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
// https://www.geeksforgeeks.org/detect-cycle-in-a-graph/

/** Todo
[x] Set up environment according to assignment spec
[x] Parse input file into graphs
    [x] Put inputs in structs
    [x] Convert struct into adjacency list 
    [x] Set up condition in case we encounter '#' char indicating end of simulation ( is.clear() )
[x] Implement DFS
    [x] Detect backedge
[x] Make distinction of process and resources of same id number
    [x] make resources of same process id diffrent (add MAXRESOURCE to each input resource and map as such)
    [x] skip the resources in the final display
[-] Bug with findDFS returning one extra node sometimes. Not an issue for this assignment though.
*/

// Imports
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <queue>
#include <sstream>
#include <string>
#include <list> 
#include <limits.h>

// Global Constants
#define DEBUG false
#define MAXPROCESS 100000 // process numbers will be in range
#define MAXRESOURCE 100000 // resource numbers will be in range
#define MAXEDGES 100000 // number of edges per state description will be in range
#define MAXSTATES 20 // number of states per input will be in range
#define ENDOFPROCESS '#'

// Global Variables

// Structs
struct ProcessResource {
    int process;
    int resource;
} stateMachine[MAXEDGES];

// Classes
class Graph  {
    int V;    // No. of vertices
    std::list<int> *adj;    // Pointer to an array containing adjacency lists
    std::queue <int> visitDFS(int v, bool visited[], bool *rs, std::queue<int> cycle);  // used by findCycle()
public:
    Graph(int V);   // Constructor
    void addEdge(int v, int w);   // to add an edge to graph
    std::queue <int> findCycle();    // returns true if there is a cycle in this graph
};

Graph::Graph(int V)  {
    this->V = V; 
    adj = new std::list<int>[V]; 
} 
  
void Graph::addEdge(int v, int w)  {
    adj[v].push_back(w); // Add w to v’s list. 
}

/**
 * Search from start vertex v
 **/
std::queue <int> Graph::visitDFS(int v, bool visited[], bool *recStack, std::queue<int> cycle)  {
    std::queue <int> iterate, buffer, debug;

    if(visited[v] == false) { 
        // Mark the current node as visited and part of recursion stack 
        visited[v] = true; 
        recStack[v] = true; 
        cycle.push(v);
        if (DEBUG) {
            printf("we added: %d in cycle: %d\n", v, cycle.front());
            std::cout << "Our current queue is" << std::endl;
            debug = cycle;
            while(!debug.empty()) {
                std::cout << debug.front() << " ";
                debug.pop();
            }
            std::cout << std::endl;
        }

        // Recur for all the vertices adjacent to this vertex 
        std::list<int>::iterator i; 
        for(i = adj[v].begin(); i != adj[v].end(); ++i) {
            if ( !visited[*i] ) {
                cycle = visitDFS(*i, visited, recStack, cycle);
                if (!cycle.empty()) {
                    return cycle; 
                }
            }
            else if (recStack[*i])
                return cycle; 
        }
    }

    if (DEBUG)
        printf("we Removed: %d in cycle: %d\n", v, cycle.front());
    
    iterate = cycle;
    while (!iterate.empty()) {
        if(iterate.front() == cycle.back())
            iterate.pop();
        else {
            buffer.push(iterate.front());
            iterate.pop();
        }
    }
    cycle.swap(buffer);

    if (DEBUG) {
        std::cout << "Our current queue is" << std::endl;
        debug = cycle;
        while(!debug.empty()) {
            std::cout << debug.front() << " ";
            debug.pop();
        }
        std::cout << std::endl;
    }
    recStack[v] = false;  // remove the vertex from recursion stack

    return cycle; 
} 
  
/**
 * Returns a graph containing the nodes that form an edge
 * it returns an empty queue if there is no cycle
 * but, there is a bug that makes it return one node sometimes
 * since we don't worry about cycles with to itself we can disregard this
 **/
std::queue <int> Graph::findCycle() {
    // Mark all the vertices as not visited and not part of recursion 
    std::queue<int> cycle;
    bool *visited = new bool[V];
    bool *recStack = new bool[V];
    for(int i = 0; i < V; i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    // Call the recursive helper function to detect cycle in different 
    // DFS trees
    for(int i = 0; i < V; i++) {
        cycle = visitDFS(i, visited, recStack, cycle);
        if (!cycle.empty())
            return cycle;
    }

    return cycle;
}

// Methods
void addEdge(std::vector<int> adj[], int u, int v);
void printGraph(std::vector< std::vector<int> > adjList);
std::vector< std::vector<int> > createAdjList(ProcessResource * imports, int size);


int main(int argc, char *argv[]) {
    int edgeCount;
    int u, v;
    char b, c;
    std::string inputLine;
    std::vector< std::vector<int> > adjList;
    std::queue <int> cycle, buffer;

    // Get input
    std::cout << "Waiting for input:" << std::endl;
    while(!std::cin.eof()) {
        edgeCount = 0;
        // Acquire nodes and edges untill we break from inside by getting a '#' or EOF
        do {
            // Check to see if line starts with '#' or EOF
            if ((std::cin.peek() == std::char_traits<char>::to_int_type(ENDOFPROCESS)) || std::cin.peek() == std::char_traits<char>::eof()) {
                std::getline(std::cin, inputLine);
                
                // Populate our graph, we use 2*MAXPROCESS to account for resources with same id as process
                // just like a relocation register
                Graph g(2*(MAXPROCESS+1));
                for(int i = 0; i < edgeCount; i++) {
                    g.addEdge(stateMachine[i].process, stateMachine[i].resource);
                }
                cycle = g.findCycle();

                // We want to ignore cycles that go through resources so we eleminate those possibilities
                while(!cycle.empty()) {
                    if (cycle.front() >= MAXRESOURCE ) {
                        cycle.pop();
                    }
                    else {
                        buffer.push(cycle.front());
                        cycle.pop();
                    }
                }
                cycle = buffer;

                // Check the remaining queue for possible cycles
                if (cycle.size() > 1) {
                    std::cout << "Deadlocked processes: ";
                    while(!cycle.empty()) {
                        std::cout << cycle.front() << " ";
                        cycle.pop();
                    }
                    std::cout << std::endl;
                }
                else
                    std::cout << "Deadlocked processes: " << "none" << std::endl;

                // Clear our queues for next itteration
                while(!cycle.empty())
                    cycle.pop();
                buffer = cycle;

                break;
            }
            else {
                std::getline(std::cin, inputLine);
                std::stringstream buffer(inputLine);
                buffer >> u >> std::ws >> b >> c >> std::ws >> v;
                
                // Add acquired edge to our struct
                if (b == '-') {
                    stateMachine[edgeCount].process = u;
                    stateMachine[edgeCount].resource = v+MAXRESOURCE;
                }
                else {
                    stateMachine[edgeCount].process = v+MAXRESOURCE;
                    stateMachine[edgeCount].resource = u;
                }
                
                ++edgeCount;
            }
        } while(1);
    }

    return 0;
}
