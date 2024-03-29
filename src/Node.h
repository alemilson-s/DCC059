/**************************************************************************************************
 * Implementation of the TAD Node
**************************************************************************************************/

#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED
#include "Edge.h" // Include of the Edge class

using namespace std;

// Definition of the Node class
class Node{

    // Attributes
    private:
        Edge* first_edge;
        Edge* last_edge;
        unsigned int object_id;
        int id;
        unsigned int in_degree;
        unsigned int out_degree;
        float weight;
        Node* next_node;

    public:
        // Constructor
        Node(int id);
        Node();
        // Destructor
        ~Node();
        // Getters
        Edge* getFirstEdge();
        Edge* getLastEdge();
        int getId();
        int getInDegree();
        int getOutDegree();
        float getWeight();
        Node* getNextNode();
        Edge* getEdge(int target_id);
        unsigned int getObjectId();
        // Setters
        void setNextNode(Node* node);
        void setWeight(float weight);
        void setObjectId(unsigned int object_id);
        // Other methods
        bool searchEdge(int target_id);
        void insertEdge(int target_id, float weight);
        void removeAllEdges();
        int removeEdge(bool directed, Node* target_node);
        void incrementOutDegree();
        void decrementOutDegree();
        void incrementInDegree();
        void decrementInDegree();
        Edge* hasEdgeBetween(int target_id);
        // Auxiliar methods
        bool containsEdge(int edge_id);
        void auxRemoveEdge(Node *node, int id_edge);

};

#endif // NODE_H_INCLUDED
