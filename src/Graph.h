/**************************************************************************************************
 * Implementation of the TAD Graph
**************************************************************************************************/

#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED
#include "Node.h"
#include <fstream>
#include <stack>
#include <list>

using namespace std;

class Graph{

    //Atributes
    private:
        int order;
        int number_edges;
        bool directed;
        bool weighted_edge;
        bool weighted_node;
        Node* first_node;
        Node* last_node;

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
        Node* getFirstNode();
        Node* getLastNode();
        //Other methods
        void insertNode(int id, bool update_order);
        void insertEdge(int id, int target_id, float weight);
        void removeNode(int id);
        bool containsNode(int id);
        Node* getNode(int id);
        bool depthFirstSearch(int initialId,int targetId);
        void breadthFirstSearch(ofstream& output_file);
        Graph* getComplement();
        Graph* getSubjacent();
        bool hasCircuit();
        bool connectedGraph();
        float** floydMarshall();
        float* dijkstra(int id);
        void generateDot(string name_graph);
        void removeEdge(int id, int target_id);
        Graph* getVertexInducedSubgraph();

    private:
        //Auxiliar methods
        bool containsEdge(int id, int target_id);
        Node* allocateNode(int id, bool update_order);

};

#endif // GRAPH_H_INCLUDED
