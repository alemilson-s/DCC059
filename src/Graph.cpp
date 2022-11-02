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

#define WHITE 0
#define YELLOW 1
#define RED 2

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
    this->id_s = 0;
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

Node *Graph::getNodeObjectId(unsigned int id) {
    Node *p = this->first_node;
    while (p != nullptr) {
        if (p->getObjectId() == id)
            return p;
        p = p->getNextNode();
    }
    return nullptr;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id, bool update_order) {
    if (this->getFirstNode() == nullptr) {
        this->first_node = this->last_node = new Node(id);
        if (update_order)
            this->order++;
        this->first_node->setObjectId(this->id_s);
        id_s++;
    } else {
        if (!this->containsNode(id)) {
            Node *p = new Node(id);
            p->setObjectId(this->id_s);
            id_s++;
            this->last_node->setNextNode(p);
            this->last_node = p;
            this->last_node->setNextNode(nullptr);
            if (update_order)
                this->order++;
        }
    }
}

Node *Graph::allocateNode(int id, bool update_order) {
    /*******************************************************************************************************************
     * método similar ao insertNode(), se difere ao retornar o nó alocado. Assim tendo como objetivo retornar o nó
     * para que ele possa ser utilizado imediatamente e não haja necessidade de busca-lo no grafo.
     * ****************************************************************************************************************/
    if (this->getFirstNode() == nullptr) {
        this->first_node = this->last_node = new Node(id);
        if (update_order)
            this->order++;
        this->first_node->setObjectId(this->id_s);
        id_s++;
        return this->first_node;
    } else {
        if (!this->containsNode(id)) {
            Node *p = new Node(id);
            p->setObjectId(this->id_s);
            id_s++;
            this->last_node->setNextNode(p);
            this->last_node = p;
            this->last_node->setNextNode(nullptr);
            if (update_order)
                this->order++;
            return this->last_node;
        }
    }
    return nullptr;
}

void Graph::insertEdge(int id, int target_id, float weight) {
    if (id == target_id)
        return;
    Node *node;
    Node *target_node;
    if (!this->containsNode(id)) {
        node = this->allocateNode(id, false);
    } else {
        node = this->getNode(id);
    }
    if (!this->containsNode(target_id)) {
        target_node = this->allocateNode(target_id, false);
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
    /*******************************************************************************************************************
     * método utilizado para remover um nó do grafo.
     * parameters:
     *      id: id do nó que será removido do grafo
     * ****************************************************************************************************************/
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


Graph *Graph::getVertexInducedSubgraph() {
    Graph *g = new Graph(0, this->directed, this->weighted_edge, this->weighted_node);
    int number_vertices, id_vertex, *vet;
    cout << "Quantidade de vértices: ";
    cin >> number_vertices;
    while (number_vertices > this->order) {
        cout << "A ordem do subgrafo vértice induzido deve ser menor ou igual a do supergrafo!" << endl;
        cout << "Digite novamente." << endl << "Quantidade de vértices: ";
        cin >> number_vertices;
    }

    vet = new int[number_vertices];

    cout
            << "Digite \"(S) ou (s)\" para fazer a leitura a partir de um arquivo, qualquer outro valor para leitura manual: ";
    string resp;
    cin >> resp;
    if (std::equal(resp.begin(), resp.end(), "S") || std::equal(resp.begin(), resp.end(), "s")) {
        cout << "Digite o caminho absoluto do arquivo: ";
        string path;
        cin >> path;
        ifstream file;
        file.open(path, ios::in);
        if (!file.is_open()) {
            while (!file.is_open()) {
                cout << "Não foi possível abrir o arquivo!" << endl;
                cout << "Tente novamente." << endl;
                cout << "Digite o caminho absoluto do arquivo: ";
                cin >> path;
                file.open(path, ios::in);
            }
        }
        int i = 0;
        while (file >> vet[i] && i < number_vertices) {
            Node *node = this->getNode(vet[i]);
            while (node == nullptr || g->containsNode(vet[i])) {
                cout << "Nó inexistente, ou já está contido no grafo!" << endl << "Digite outro nó." << endl;
                cout << "Id " << i + 1 << "o vértice: ";
                cin >> vet[i];
                node = this->getNode(vet[i]);
            }
            Node *p = g->allocateNode(vet[i], true);
            p->setWeight(node->getWeight());
            i++;
        }
        file.close();
    } else {
        for (int i = 0; i < number_vertices; i++) {
            cout << "Id " << i + 1 << "o vértice: ";
            cin >> id_vertex;
            Node *node = this->getNode(id_vertex);
            while (node == nullptr || g->containsNode(id_vertex)) {
                cout << "Nó inexistente, ou já está contido no grafo!" << endl << "Digite novamente." << endl;
                cout << "Id " << i + 1 << "o vértice: ";
                cin >> id_vertex;
                node = this->getNode(id_vertex);
            }
            vet[i] = id_vertex;
            Node *p = g->allocateNode(id_vertex, true);
            p->setWeight(node->getWeight());
        }
    }

    for (Node *p = g->getFirstNode(); p != nullptr; p = p->getNextNode()) {
        Node *node = this->getNode(p->getId());
        for (int i = 0; i < number_vertices; i++) {
            Edge *edge = node->getEdge(vet[i]);
            if (edge != nullptr)
                p->insertEdge(vet[i], edge->getWeight());
        }
    }
    delete[]vet;
    return g;
}

void Graph::visitNode(int id, int *colors) {
    colors[id] = YELLOW;
    Node *p = this->getNodeObjectId(id);
    Edge *t = nullptr;
    if (p != nullptr)
        t = p->getFirstEdge();
    while (t != nullptr) {
        Node *aux = this->getNode(t->getTargetId());
        if (colors[aux->getObjectId()] == WHITE)
            visitNode(aux->getObjectId(), colors);
        t = t->getNextEdge();
    }
    colors[id] = RED;
}

void Graph::depthSearch() {
    int colors[this->order];

    for (int i = 0; i < this->order; i++)
        colors[i] = WHITE;
    for (int i = 0; i < this->order; i++) {
        if (colors[i] == WHITE) {
            visitNode(i, colors);
        }
    }
}

void
Graph::visitNode(int id, int *colors, int time, int accumulated_time, list<int> path, executionPath *execution, int *visits) {
    colors[id] = YELLOW;
    Node *p = this->getNodeObjectId(id);
    visits[p->getObjectId()]++;
    Edge *t = nullptr;
    if (p != nullptr)
        t = p->getFirstEdge();
    accumulated_time += time;
    path.push_back(id);
    if (execution->weight < accumulated_time) {
        execution->weight = accumulated_time;
        execution->path.clear();
        list<int>::iterator it;
        for (it = path.begin(); it != path.end(); it++)
            execution->path.push_back(*it);
    }
    while (t != nullptr) {
        Node *aux = this->getNode(t->getTargetId());
        if (colors[aux->getObjectId()] == WHITE || visits[aux->getObjectId()] <= p->getInDegree())
            visitNode(aux->getObjectId(), colors, t->getWeight(), accumulated_time, path, execution, visits);
        if (colors[aux->getObjectId()] == YELLOW) {
            cout << "Grafo possui ciclo!" << endl;
            exit(55);
        }
        t = t->getNextEdge();
    }
    colors[id] = RED;

}

void Graph::closeNetwork() {
    if (!this->directed) {
        cout << "Grafo deve ser direcionado!" << endl;
        return;
    }
    int time = 0;
    int colors[this->order];
    executionPath *execution = new executionPath;
    execution->weight = 0;
    list<int> path;
    int *visits = new int[this->order];
    for (int i = 0; i < this->order; i++) {
        colors[i] = WHITE;
        visits[i] = 0;
    }
    for (int i = 0; i < this->order; i++) {
        if (colors[i] == WHITE) {
            visitNode(i, colors, time, 0, path, execution, visits);
        }
    }
    list<int>::iterator it;
    for (it = execution->path.begin(); it != execution->path.end(); it++)
        cout << "-> etapa: " << this->getNodeObjectId(*it)->getId() << endl;
    cout << "Duração do projeto: " << execution->weight << endl;
}

int *Graph::getIndirectTransitiveClosure(int id_node) {
    Node *node = this->getFirstNode();
    int *vet = new int[this->order], i;
    for (i = 0; i < this->order; i++)
        vet[i] = -1;
    i = 0;
    while (node != nullptr) {
        if (node->containsEdge(id_node)) {
            vet[i] = node->getId();
            i++;
        }
        node = node->getNextNode();
    }
    return vet;
}

Graph *Graph::intersection(Graph *g) {
    Graph *p = new Graph(0, this->directed, this->weighted_edge, this->weighted_node);
    Node *node = this->getFirstNode();
    while (node != nullptr) {
        if (g->containsNode(node->getId())) {
            Node *n = p->allocateNode(node->getId(), true);
            n->setWeight(node->getWeight());
            Edge *t = node->getFirstEdge();
            while (t != nullptr) {
                if (node->containsEdge(t->getTargetId())) {
                    n->insertEdge(t->getTargetId(), t->getWeight());
                }
                t = t->getNextEdge();
            }
        }
        node = node->getNextNode();
    }
    return p;
}

void Graph::generateDot(string name_graph) {
    /*******************************************************************************************************************
     * método utilizado para gerar o arquivo "*.dot".
     * parameters:
     *      name_graph: string utilizada como nome do arquivo que será gerado
     * ****************************************************************************************************************/
    string path = "../output_files/";
    path.append(name_graph).append("_graph.dot");
    fstream output_file;
    output_file.open(path, ios::out);
    if (!output_file.is_open()) {
        path = "./output_files/";
        path.append(name_graph).append("_graph.dot");
        output_file.open(path, ios::out);
    }

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
        cout << "Arquivo " << path << " gerado!" << endl;
        /*
        name_graph.append("_graph.png");

        string command = "dot -Tpng ";
        command.append(path.append(" -o ../output_files/").append(name_graph));

        char *c = const_cast<char *>(command.c_str());

        if (!(c = reinterpret_cast<char *>((FILE *) popen(c, "r")))) {

            cout << "falha";

            exit(1);
        }
         */
        output_file.close();

    } else {
        cout << "An error occurred while trying to open the file!" << endl << "generateDot()" << endl;
    }
}