/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "Node.h"
#include <fstream>
#include <stack>
#include <list>
#include <vector>

typedef struct {
    float weight;
    list<int> path;
} executionPath;

using namespace std;

class Graph {

    //Atributes
private:
    int order;
    int number_edges;
    bool directed;
    bool weighted_edge;
    bool weighted_node;
    Node *first_node;
    Node *last_node;
    int id_s;

public:
    //Constructor
    Graph(int order, bool directed, bool weighted_edge, bool weighted_node);

    //Destructor
    ~Graph();

    //Getters
    int getOrder();

    int getNumberEdges();

    bool getDirected();

    bool getWeightedEdge();

    bool getWeightedNode();

    Node *getFirstNode();

    Node *getLastNode();

    Node *getNodeObjectId(unsigned int id);

    //Other methods
    void insertNode(int id, bool update_order);

    void insertEdge(int id, int target_id, float weight);

    void removeNode(int id);

    bool containsNode(int id);

    Node *getNode(int id);

    bool depthFirstSearch(int initialId, int targetId);

    void breadthFirstSearch(ofstream &output_file);

    Graph *getComplement();

    Graph *getSubjacent();

    bool hasCircuit();

    bool connectedGraph();

    float **floydMarshall();

    float *dijkstra(int id);

    void generateDot(string name_graph);

    void removeEdge(int id, int target_id);

    Graph *getVertexInducedSubgraph();

    void depthSearch();

    void visitNode(int id, int *colors);

    int *getIndirectTransitiveClosure(int id_node);

    Graph *intersection(Graph *g);

    Graph *unionGraph(Graph *g);

    Graph *differenceGraph(Graph *g);

    void pertNetwork();

    void stepMetrics(int id, int *colors, int time, int accumulated_time, list<int> path, executionPath *execution,
                     int *visits);

    void print();

    Node *allocateNode(int id, bool update_order);

    void greedyConstructiveAlgorithm();

    void atualizaLista(vector<int> *lista_candidatos, int *degrees);

private:
    //Auxiliar methods
    bool containsEdge(int id, int target_id);

};

#endif // GRAPH_H_INCLUDED
