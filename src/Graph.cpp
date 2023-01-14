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
#include "random.h"
#include "types.h"
#include <math.h>
#include <random>
#include <chrono>


using namespace std::chrono;

using namespace std;

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
        if (!node->containsEdge(target_id)) {
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
    if (node == nullptr)
        return false;
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
    cout << "Subgrafo Induzido" << endl;
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
            if (edge != nullptr && g->containsNode(edge->getTargetId()))
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
Graph::stepMetrics(int id, int *colors, int time, int accumulated_time, list<int> path, executionPath *execution,
                   int *visits) {
    colors[id] = YELLOW;
    Node *p = this->getNodeObjectId(id);
    Edge *t = nullptr;
    if (p != nullptr)
        t = p->getFirstEdge();
    else
        return;
    visits[p->getObjectId()]++;
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
            stepMetrics(aux->getObjectId(), colors, t->getWeight(), accumulated_time, path, execution, visits);
        if (colors[aux->getObjectId()] == YELLOW) {
            cout << "Grafo possui ciclo!" << endl;
            exit(55);
        }
        t = t->getNextEdge();
    }
    colors[id] = RED;

}

void Graph::pertNetwork() {
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
            stepMetrics(i, colors, time, 0, path, execution, visits);
        }
    }
    list<int>::iterator it;
    cout << endl << "Ordem das tarefas:" << endl;
    list<int>::iterator aux = execution->path.begin();
    aux++;
    for (it = execution->path.begin(); aux != execution->path.end(); it++) {
        cout << "(" << this->getNodeObjectId(*it)->getId() << ", " << this->getNodeObjectId(*aux)->getId() << ")"
             << endl;
        aux++;
    }
    cout << "Duração do projeto: " << execution->weight << endl << endl;
    delete execution;
    delete[]visits;
}

Graph *Graph::intersection(Graph *g) {
    Graph *p = new Graph(0, this->directed, this->weighted_edge, this->weighted_node);
    Node *node = this->getFirstNode();
    while (node != nullptr) {
        if (g->containsNode(node->getId())) {
            Node *n = p->allocateNode(node->getId(), true);
            n->setWeight(node->getWeight());
        }
        node = node->getNextNode();
    }
    node = this->getFirstNode();
    while (node != nullptr) {
        Node *node_g = g->getNode(node->getId());
        Edge *t = node->getFirstEdge();
        while (t != nullptr) {
            if (node_g != nullptr)
                if (node_g->containsEdge(t->getTargetId())) {
                    Node *n = p->getNode(node->getId());
                    n->insertEdge(t->getTargetId(), t->getWeight());
                }
            t = t->getNextEdge();
        }
        node = node->getNextNode();
    }

    p->generateDot("intersection_graph");
    cout << endl << "Grafo Interseção: " << endl;
    this->print();
    return p;
}

Graph *Graph::unionGraph(Graph *g) {
    Graph *p = new Graph(0, this->directed, this->weighted_edge, this->weighted_node);
    Node *node = this->getFirstNode();
    while (node != nullptr) {
        p->insertNode(node->getId(), true);
        node = node->getNextNode();
    }
    node = this->getFirstNode();
    while (node != nullptr) {
        Edge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            if (!p->containsEdge(node->getId(), edge->getTargetId())) {
                p->insertEdge(node->getId(), edge->getTargetId(), edge->getWeight());
            }
            edge = edge->getNextEdge();
        }
        node = node->getNextNode();
    }

    node = g->getFirstNode();
    while (node != nullptr) {
        if (!p->containsNode(node->getId()))
            p->insertNode(node->getId(), true);
        node = node->getNextNode();
    }

    node = g->getFirstNode();
    while (node != nullptr) {
        Edge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            if (!p->containsEdge(node->getId(), edge->getTargetId())) {
                p->insertEdge(node->getId(), edge->getTargetId(), edge->getWeight());
            }
            edge = edge->getNextEdge();
        }
        node = node->getNextNode();
    }
    p->generateDot("union_graph");
    cout << endl << "Grafo União: " << endl;
    this->print();
    return p;
}

Graph *Graph::differenceGraph(Graph *g) {
    Graph *p = new Graph(0, this->directed, this->weighted_edge, this->weighted_node);

    Node *node = g->getFirstNode();
    while (node != nullptr) {
        Edge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            if (!this->containsEdge(node->getId(), edge->getTargetId())) {
                cout << "As arestas de G2 devem estar contidas em G1!" << endl;
                return nullptr;
            }
            edge = edge->getNextEdge();
        }
        node = node->getNextNode();
    }

    node = this->getFirstNode();
    while (node != nullptr) {
        p->insertNode(node->getId(), true);
        node = node->getNextNode();
    }
    node = this->getFirstNode();
    while (node != nullptr) {
        Edge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            if (!g->containsEdge(node->getId(), edge->getTargetId())) {
                p->insertEdge(node->getId(), edge->getTargetId(), edge->getWeight());
            }
            edge = edge->getNextEdge();
        }
        node = node->getNextNode();
    }
    p->generateDot("difference_graph");
    cout << endl << "Grafo Diferença: " << endl;
    this->print();
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
//        name_graph.append("_graph.png");
//
//        string command = "dot -Tpng ";
//        command.append(path.append(" -o ../output_files/").append(name_graph));
//
//        char *c = const_cast<char *>(command.c_str());
//
//        if (!(c = reinterpret_cast<char *>((FILE *) popen(c, "r")))) {
//
//            cout << "falha";
//
//            exit(1);
//        }
        output_file.close();

    } else {
        cout << "An error occurred while trying to open the file!" << endl << "generateDot()" << endl;
    }
}

void Graph::print() {
    Node *node = this->getFirstNode();
    while (node != nullptr) {
        cout << "\tNode id: " << node->getId() << endl;
        cout << "\tEdges:" << endl;
        Edge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            cout << "\t\t(" << node->getId() << ", " << edge->getTargetId() << ")" << endl;
            edge = edge->getNextEdge();
        }
        node = node->getNextNode();
    }
    cout << endl;
}

void merge(vector<int> *nodes, double *f, int *auxNodes, double *auxF, int l, int m, int r) {
    int i = l;
    int j = m + 1;
    int k = 0;
    while (i <= m && j <= r) {
        if (f[j] <= f[i]) {
            auxNodes[k] = (*nodes)[i];
            auxF[k] = f[i];
            i++;
        } else {
            auxNodes[k] = (*nodes)[j];
            auxF[k] = f[j];
            j++;
        }
        k++;
    }

    while (i <= m) {
        auxNodes[k] = (*nodes)[i];
        auxF[k] = f[i];
        i++;
        k++;
    }

    while (j <= r) {
        auxNodes[k] = (*nodes)[j];
        auxF[k] = f[j];
        j++;
        k++;
    }

    for (i = l; i <= r; i++) {
        (*nodes)[i] = auxNodes[i - l];
        f[i] = auxF[i - l];
    }
}

void mergeSort(vector<int> *nodes, double *f, int *auxNodes, double *auxF, int first, int last) {
    if (first < last) {
        int mid = (first + last) / 2;
        mergeSort(nodes, f, auxNodes, auxF, first, mid);
        mergeSort(nodes, f, auxNodes, auxF, mid + 1, last);
        merge(nodes, f, auxNodes, auxF, first, mid, last);
    }
}

bool Graph::isNotCovered(solucao &s, int &id_node) {
    bool coberto = false;
    bool adjacentes_coberto = true;
    for (int i = 0; i < s.solucao.size(); i++) {
        Node *node = this->getNode(s.solucao[i]);
        if (node->containsEdge(id_node)) {
            coberto = true;
            break;
        }
    }
    Node *node = this->getNode(id_node);
    Edge *edge = node->getFirstEdge();
    while (edge != nullptr) {
        for (int i = 0; i < s.solucao.size(); i++) {
            Node *n = this->getNode(s.solucao[i]);
            if (!n->containsEdge(edge->getTargetId())) {
                adjacentes_coberto = false;
                break;
            }
        }
        if (!adjacentes_coberto)
            break;
        edge = edge->getNextEdge();
    }
    if (coberto && adjacentes_coberto)
        return true;
    else
        return false;
}

void Graph::atualizaLista(vector<int> *lista_candidatos, int *degrees, int ind, int tam) {
    Node *node = this->getNode((*lista_candidatos)[ind]);

    lista_candidatos->erase(lista_candidatos->begin() + ind);
    Edge *edge = node->getFirstEdge();
    while (edge != nullptr) {
        degrees[edge->getTargetId()]--;
        edge = edge->getNextEdge();
    }
    for (int i = 0; i < tam; i++) {
        if (degrees[i] == 0) {
            for (int j = 0; j < lista_candidatos->size(); j++) {
                if ((*lista_candidatos)[j] == i)
                    lista_candidatos->erase(lista_candidatos->begin() + j);
            }
            degrees[i] = -1;
            i--;
        }
    }

}

void Graph::inicializaF(double *f, int *weights, int tam) {
    Node *node;
    Edge *edge;
    for (int i = 0; i < tam; i++) {
        f[i] = 0;
        node = this->getNode(i);
        f[i] += node->getWeight();
        edge = node->getFirstEdge();
        float peso;
        while (edge != nullptr) {
            peso = this->getNode(edge->getTargetId())->getWeight();
            f[i] += peso;
            edge = edge->getNextEdge();
        }
        f[i] = (f[i] / node->getInDegree()) / ((weights[i]));
    }
}

void Graph::greedyConstructiveAlgorithm(ofstream &output_file) {
    output_file << "ALGORITMO CONSTRUTIVO GULOSO" << endl;
    cout << "ALGORITMO CONSTRUTIVO GULOSO" << endl;
    high_resolution_clock::time_point start = high_resolution_clock::now();
    int weights[this->order];
    int degrees[this->order];
    double f[this->order];
    int auxNodes[this->order];
    double auxF[this->order];
    vector<int> lista_candidatos;
    vector<int> solucao;
    for (int i = 0; i < this->order; i++) {
        Node *node = this->getNode(i);
        weights[i] = node->getWeight();
        degrees[i] = node->getInDegree();
    }
    inicializaF(f, weights, this->order);
    for (int i = 0; i < this->order; i++) {
        Node *p = this->getNode(i);
        if (p->getInDegree() == 0) {
            solucao.push_back(i);
        } else
            lista_candidatos.push_back(i);
    }
    mergeSort(&lista_candidatos, f, auxNodes, auxF, 0, this->order - 1);
    while (lista_candidatos.size() > 0) {
        solucao.push_back(lista_candidatos[0]);
        atualizaLista(&lista_candidatos, degrees, 0, this->order);
    }
    float qualidade_solucao = 0;
    output_file << "Solução:" << endl;
    for (int i = 0; i < solucao.size(); i++) {
        Node *node = this->getNode(solucao[i]);
        output_file << "\t" << solucao[i] << endl;
        qualidade_solucao += node->getWeight();
    }

    output_file << "Qualidade final da solução: " << qualidade_solucao << endl;
    high_resolution_clock::time_point end = high_resolution_clock::now();
    output_file << "Tempo de processamento: " << duration_cast<chrono::milliseconds>(end - start).count()
                << " milliseconds"
                << endl;

    cout << "Qualidade final da solução: " << qualidade_solucao << endl;
    cout << "Tempo de processamento: " << duration_cast<chrono::milliseconds>(end - start).count() << " milliseconds"
         << endl;
    cout << "Solução viável" << endl << endl;
}

solucao *Graph::adaptiveRandomizedGreedyAlgorithm(float alpha, int numero_iteracoes, ofstream &output_file,
                                                  bool gravar_solucao) {
    high_resolution_clock::time_point start = high_resolution_clock::now();
    if (gravar_solucao) {
        output_file << "ALGORITMO CONSTRUTIVO GULOSO RANDOMIZADO E ADAPTATIVO" << endl;
        cout << "ALGORITMO CONSTRUTIVO GULOSO RANDOMIZADO E ADAPTATIVO" << endl;
    }
    solucao *best = new solucao;
    best->valor_solucao = 0;
    for (Node *node = this->getFirstNode(); node != nullptr; node = node->getNextNode())
        best->valor_solucao += node->getWeight();
    int j = 0;
    ULONG k;
    while (j < numero_iteracoes) {
        j++;
        int weights[this->order];
        int degrees[this->order];
        double f[this->order];
        int auxNodes[this->order];
        double auxF[this->order];
        vector<int> lista_candidatos;
        solucao *s = new solucao;
        s->valor_solucao = 0;
        for (int i = 0; i < this->order; i++) {
            Node *node = this->getNode(i);
            weights[i] = node->getWeight();
            degrees[i] = node->getInDegree();
        }

        for (int i = 0; i < this->order; i++) {
            Node *p = this->getNode(i);
            if (p->getInDegree() == 0) {
                s->solucao.push_back(i);
            } else
                lista_candidatos.push_back(i);
        }
        inicializaF(f, weights, this->order);
        mergeSort(&lista_candidatos, f, auxNodes, auxF, 0, this->order - 1);
        do {
            srand(time(nullptr));
            int aux = alpha * (lista_candidatos.size() - 1);
            if (aux == 0)
                k = 0;
            else
                k = rand() % aux;
            if (!isNotCovered(*s, lista_candidatos[k]) && degrees[lista_candidatos[k]] > 0)
                s->solucao.push_back(lista_candidatos[k]);
            atualizaLista(&lista_candidatos, degrees, k, this->order);
        } while (lista_candidatos.size() > 0);
        for (int i = 0; i < s->solucao.size(); i++) {
            Node *node = this->getNode(s->solucao[i]);
            s->valor_solucao += node->getWeight();
        }
        if (s->valor_solucao < best->valor_solucao) {
            solucao *aux = best;
            best = s;
            delete aux;
        } else {
            delete s;
        }
    }
    float qualidade_solucao = 0;
    if (gravar_solucao) { output_file << "Solução(semente de randomização - " << alpha << "):" << endl; }
    for (int i = 0; i < best->solucao.size(); i++) {
        Node *node = this->getNode(best->solucao[i]);
        if (gravar_solucao)
            output_file << "\t" << best->solucao[i] << endl;
        qualidade_solucao += node->getWeight();
    }
    if (gravar_solucao)
        output_file << "Qualidade final da solução: " << qualidade_solucao << endl;
    high_resolution_clock::time_point end = high_resolution_clock::now();
    if (gravar_solucao)
        output_file << "Tempo de processamento: " << duration_cast<chrono::milliseconds>(end - start).count()
                    << " milliseconds"
                    << endl;
    if (gravar_solucao) {
        cout << "Semente de randomização: " << alpha << endl;
        cout << "Qualidade final da solução: " << qualidade_solucao << endl;
        cout << "Tempo de processamento: " << duration_cast<chrono::milliseconds>(end - start).count()
             << " milliseconds" << endl;
        cout << "Solução viável" << endl << endl;
    }
    return best;
}

void Graph::inicializaVetores(vector<float> &p, vector <media> &m, vector <solucao> &best_solutions, int tam) {
    media aux_medias{0, 0, 1};
    int qtd_alfas = tam;
    solucao *aux_solucao = new solucao;
    float aux_probabilidades = 1.0f / qtd_alfas;
    float valor_melhor_solucao = 0;
    for (Node *node = this->getFirstNode(); node != nullptr; node = node->getNextNode()) {
        aux_solucao->solucao.push_back(node->getId());
        valor_melhor_solucao += node->getWeight();
    }
    aux_solucao->valor_solucao = valor_melhor_solucao;
    for (int i = 0; i < qtd_alfas; i++) {
        best_solutions.push_back(*aux_solucao);
        p.push_back(aux_probabilidades);
        m.push_back(aux_medias);
    }

}

void Graph::atualizaProbabilidades(vector<float> &p, vector <media> &m, vector <solucao> &best_solutions) {
    long double q[p.size()];
    long double qualidade_melhor_solucao = best_solutions[0].valor_solucao, soma = 0;
    for (int i = 1; i < best_solutions.size(); i++) {
        if (best_solutions[i].valor_solucao < qualidade_melhor_solucao)
            qualidade_melhor_solucao = best_solutions[i].valor_solucao;
    }

    for (int i = 0; i < p.size(); i++) {
        q[i] = pow(qualidade_melhor_solucao / m[i].media, 100);
        soma += q[i];
    }
    for (int i = 0; i < p.size(); i++) {
        p[i] = q[i] / soma;
    }
}

int Graph::escolheAlfa(vector<float> &p, vector<float> &a) {
    float aux[p.size()];
    for (int i = 0; i < p.size(); i++) {
        aux[i] = p[i] * 1000;
    }
    float t = 0;
    int r = rand() % 1001;
    for (int i = 0; i < p.size(); i++) {
        t += aux[i];
        if (t >= r)
            return i;
    }
    return 0;
}


void Graph::atualizaMedias(vector <media> &medias, float valor_solucao, int index_alpha) {
    medias[index_alpha].soma += valor_solucao;
    medias[index_alpha].qtd_solucoes++;
    medias[index_alpha].media = medias[index_alpha].soma / medias[index_alpha].qtd_solucoes;
}

void Graph::reactiveRandomizedGreedyConstructiveAlgorithm(vector<float> alphas, int numero_iteracoes, int bloco,
                                                          int tam, ofstream &output_file) {
    high_resolution_clock::time_point start = high_resolution_clock::now();
    output_file << "ALGORITMO CONSTRUTIVO GULOSO RANDOMIZADO REATIVO" << endl;
    cout << "ALGORITMO CONSTRUTIVO GULOSO RANDOMIZADO REATIVO" << endl;
    vector <solucao> best_solutions;
    solucao *solution;
    int j = 0;
    int index_best_alpha = 0;
    vector<float> probabilidades;
    vector <media> medias;

    inicializaVetores(probabilidades, medias, best_solutions, tam);
    while (j < numero_iteracoes) {
        if (j % bloco == 0)
            atualizaProbabilidades(probabilidades, medias, best_solutions);
        j++;
        int index_alpha = escolheAlfa(probabilidades, alphas);
        solution = this->adaptiveRandomizedGreedyAlgorithm(alphas[index_alpha], 1, output_file, false);
        atualizaMedias(medias, solution->valor_solucao, index_alpha);
        if (solution->valor_solucao < best_solutions[index_alpha].valor_solucao) {
            solucao *s = solution;
            best_solutions[index_alpha] = *solution;
            delete s;
        } else {
            delete solution;
        }
    }
    high_resolution_clock::time_point end = high_resolution_clock::now();
    float best_alpha = alphas[0];
    for (int i = 0; i < alphas.size(); i++) {
        if (best_solutions[i].valor_solucao < best_solutions[index_best_alpha].valor_solucao) {
            index_best_alpha = i;
        }
    }
    output_file << "Alfa: " << alphas[index_best_alpha] << endl;
    output_file << "Solução(semente de randomização - " << alphas[index_best_alpha] << "):" << endl;
    for (int i = 0; i < best_solutions[index_best_alpha].solucao.size(); i++) {
        Node *p = this->getNode(best_solutions[index_best_alpha].solucao[i]);
        output_file << "\t" << p->getId() << endl;
    }
    output_file << "Qualidade final da solução: " << best_solutions[index_best_alpha].valor_solucao << endl;
    output_file << "Tempo de processamento: " << duration_cast<chrono::seconds>(end - start).count() << " seconds"
                << endl;
    cout << "Semente de randomização: " << alphas[index_best_alpha] << endl;
    cout << "Qualidade final da solução: " << best_solutions[index_best_alpha].valor_solucao << endl;
    cout << "Tempo de processamento: " << duration_cast<chrono::seconds>(end - start).count() << " seconds" << endl;
    cout << "Solução viável" << endl << endl;

}