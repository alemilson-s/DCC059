#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node) {

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph() {

    Node *next_node = this->first_node;

    while (next_node != nullptr) {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
    this->order = 0;
}

// Getters
int Graph::getOrder() {

    return this->order;
}

int Graph::getNumberEdges() {

    return this->number_edges;
}

bool Graph::getDirected() {

    return this->directed;
}

bool Graph::getWeightedEdge() {

    return this->weighted_edge;
}

bool Graph::getWeightedNode() {

    return this->weighted_node;
}

Node *Graph::getFirstNode() {

    return this->first_node;
}

Node *Graph::getLastNode() {

    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id) {
    if (this->getFirstNode() == nullptr) {
        this->first_node = this->last_node = new Node(id);
    } else {
        if (!this->containsNode(id)) {
            Node *p = new Node(id);
            this->last_node->setNextNode(p);
            this->last_node = p;
            this->last_node->setNextNode(nullptr);
        }
    }
}

Node *Graph::allocateNode(int id) {
    if (this->getFirstNode() == nullptr) {
        this->first_node = this->last_node = new Node(id);
        return this->first_node;
    } else {
        if (!this->containsNode(id)) {
            Node *p = new Node(id);
            this->last_node->setNextNode(p);
            this->last_node = p;
            this->last_node->setNextNode(nullptr);
            return this->last_node;
        }
    }
    return nullptr;
}

void Graph::insertEdge(int id, int target_id, float weight) {
    Node *node;
    Node *target_node;
    if (!this->containsNode(id)) {
        node = this->allocateNode(id);
    } else {
        node = this->getNode(id);
    }
    if (!this->containsNode(target_id)) {
        target_node = this->allocateNode(target_id);
    } else {
        target_node = this->getNode(target_id);
    }

    if (this->getDirected()) {
        if (!node->containsEdge(id))
            node->insertEdge(target_id, weight);

        node->incrementOutDegree();
        target_node->incrementInDegree();
    } else {
        if (!node->containsEdge(id)) {
            node->insertEdge(target_id, weight);
            target_node->insertEdge(id, weight);
            node->incrementOutDegree();
            target_node->incrementOutDegree();
            node->incrementInDegree();
            target_node->incrementInDegree();
        }
    }
    this->number_edges++;
}

bool Graph::containsEdge(int id, int target_id) {
    Node *node = this->getNode(id);
    Edge *edge = node->getFirstEdge();

    while (edge != nullptr) {
        if (edge->getTargetId() == target_id) {
            return true;
        }
        edge = edge->getNextEdge();
    }

    return false;
}

void Graph::removeNode(int id) {
    if (this->containsNode(id)) {
        Node *node = this->getNode(id);
        Edge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            Edge *aux = edge;
            edge = edge->getNextEdge();
            node->removeEdge(this->directed, this->getNode(aux->getTargetId()));
        }

        Node *aux = this->getFirstNode();
        Node *previous = nullptr;

        while (aux->getId() != id) {
            previous = aux;
            aux = aux->getNextNode();
        }

        if (previous != nullptr) {
            previous->setNextNode(aux->getNextNode());
        } else {
            this->first_node = aux->getNextNode();
        }
        if (aux == this->last_node) {
            this->last_node = previous;
        }
        if (aux->getNextNode() == this->last_node) {
            previous->setNextNode(last_node);
        }
        this->order--;
    }

}

void Graph::removeEdge(int id, int target_id) {
    Node *node = this->getNode(id);
    node->removeEdge(this->directed, this->getNode(target_id));
    this->number_edges--;
}

bool Graph::containsNode(int id) {
    Node *node = this->getFirstNode();
    while (node != nullptr) {
        if (node->getId() == id)
            return true;
        node = node->getNextNode();
    }
    return false;
}

Node *Graph::getNode(int id) {
    Node *node = this->getFirstNode();

    while (node != nullptr) {
        if (node->getId() == id)
            return node;
        node = node->getNextNode();
    }
    return nullptr;
}


//Function that verifies if there is a path between two nodes
bool Graph::depthFirstSearch(int initialId, int targetId) {
    return false;
}


void Graph::breadthFirstSearch(ofstream &output_file) {

}


Graph *Graph::getComplement() {
    return nullptr;
}


//A function that returns a subjacent of a directed graph, which is a graph which the arcs have opposite directions to the original graph
Graph *Graph::getSubjacent() {
    return nullptr;
}

bool Graph::connectedGraph() {
    return false;
}


bool Graph::hasCircuit() {
    return false;
}


float **Graph::floydMarshall() {
    return 0;
}


float *Graph::dijkstra(int id) {
    return 0;
}

void Graph::generateDot(string name_graph) {
    string path = "../output_files/";
    path.append(name_graph).append("_graph.dot");
    fstream output_file;
    output_file.open(path, ios::out);

    if (output_file.is_open()) {
        if (this->directed) {
            output_file << "strict digraph " << name_graph << "{\n";
            for (Node *p = this->getFirstNode(); p != nullptr; p = p->getNextNode()) {
                output_file << p->getId() << "\n";
                for (Edge *t = p->getFirstEdge(); t != nullptr; t = t->getNextEdge()) {
                    if (this->weighted_edge) {
                        output_file << p->getId() << "->" << t->getTargetId() << "[weight=" << t->getWeight()
                                    << ", label=" << t->getWeight() << ", color=blue];\n";
                    } else {
                        output_file << p->getId() << "->" << t->getTargetId() << ";\n";
                    }

                }
            }
            output_file << "}";
        } else {
            output_file << "strict graph " << name_graph << "{\n";
            for (Node *p = this->getFirstNode(); p != nullptr; p = p->getNextNode()) {
                output_file << p->getId() << "\n";
                for (Edge *t = p->getFirstEdge(); t != nullptr; t = t->getNextEdge()) {
                    if (this->weighted_edge) {
                        output_file << p->getId() << "--" << t->getTargetId() << "[weight=" << t->getWeight()
                                    << ", label=" << t->getWeight() << ", color=blue];\n";
                    } else {
                        output_file << p->getId() << "--" << t->getTargetId() << ";\n";
                    }
                }
            }
            output_file << "}";
        }

        name_graph.append("_graph.png");

        string command = "dot -Tpng ";
        command.append(path.append(" -o ../output_files/").append(name_graph));

        char *c = const_cast<char *>(command.c_str());

        if (!(c = reinterpret_cast<char *>((FILE *) popen(c, "r")))) {

            cout << "falha";

            exit(1);
        }
        output_file.close();

    } else {
        cout << "An error occurred while trying to open the file!" << endl;
    }
}