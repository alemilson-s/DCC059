#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"
#include "random.h"

using namespace std;

Graph *leitura(ifstream &input_file, int directed, int weightedEdge, int weightedNode) {

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Graph *graph = new Graph(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo

    if (!graph->getWeightedEdge() && !graph->getWeightedNode()) {

        while (input_file >> idNodeSource >> idNodeTarget) {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);

        }

    } else if (graph->getWeightedEdge() && !graph->getWeightedNode()) {

        float edgeWeight;

        while (input_file >> idNodeSource >> idNodeTarget >> edgeWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);

        }

    } else if (graph->getWeightedNode() && !graph->getWeightedEdge()) {

        float nodeSourceWeight, nodeTargetWeight;

        while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, 0);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    } else if (graph->getWeightedNode() && graph->getWeightedEdge()) {

        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {

            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);

        }

    }

    return graph;
}

Graph *
leituraInstanciaSubconjuntoDominantePonderado(ifstream &input_file, int directed, int weightedEdge, int weightedNode) {

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    string line;
    getline(input_file, line);
    while (line.find("Positions") == std::string::npos) {
        order = atoi(line.c_str());
        getline(input_file, line);
    }

    getline(input_file, line);
    while (line.find("WEIGHTS") == std::string::npos) {
        getline(input_file, line);
    }

    //Criando objeto grafo
    Graph *graph = new Graph(order, directed, weightedEdge, weightedNode);
    getline(input_file, line);
    //Leitura de arquivo
    int i = 0;
    while (line.find("CONNECTIONS") == std::string::npos) {
        graph->insertNode(i, false);
        Node *node = graph->getNode(i);
        node->setWeight(atoi(line.c_str()));
        getline(input_file, line);
        i++;
    }

    for (i = 0; i < graph->getOrder(); i++) {
        int j = 0;
        while (j < graph->getOrder() && input_file >> idNodeTarget) {
            if (i != j && idNodeTarget == 1) {
                graph->insertEdge(i, j, 0);
            }
            j++;
        }

    }
    return graph;
}

int menu() {

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Grafo Interseção" << endl;
    cout << "[2] Grafo União" << endl;
    cout << "[3] Grafo Diferença" << endl;
    cout << "[4] Rede Pert" << endl;
    cout << "[5] Algoritmos Gulosos (Abre um submenu)" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;

}

void selecionar(int selecao, Graph *graph, ofstream &output_file) {

    switch (selecao) {

        //Interseção
        case 1: {
            cout << "Vértices para grafo G1: " << endl;
            Graph *g1 = graph->getVertexInducedSubgraph();
            cout << "Vértices para grafo G2: " << endl;
            Graph *g2 = graph->getVertexInducedSubgraph();
            g1->intersection(g2);
            break;
        }

            //União
        case 2: {
            cout << "Vértices para grafo G1: " << endl;
            Graph *g1 = graph->getVertexInducedSubgraph();
            cout << "Vértices para grafo G2: " << endl;
            Graph *g2 = graph->getVertexInducedSubgraph();
            g1->unionGraph(g2);
            break;
        }

            //Diferença
        case 3: {
            cout << "Vértices para grafo G1: " << endl;
            Graph *g1 = graph->getVertexInducedSubgraph();
            cout << "Vértices para grafo G2: " << endl;
            Graph *g2 = graph->getVertexInducedSubgraph();
            g1->differenceGraph(g2);
            break;
        }

            //Rede Pert
        case 4: {
            char r;
            cout << "Obter subgrafo?" << endl;
            cout << "(S ou s) para sim" << endl;
            cout << "R: ";
            cin >> r;
            if (r == 'S' || r == 's') {
                cout << "Vértices para grafo acíclico direcionado: " << endl;
                Graph *g1 = graph->getVertexInducedSubgraph();
                g1->pertNetwork();
            } else
                graph->pertNetwork();
            break;
        }


        case 5: {
            cout << "Algoritmos Gulosos:" << endl;
            cout << "[1] Algoritmo construtivo guloso" << endl;
            cout << "[2] Algoritmo construtivo guloso randomizado e adaptativo" << endl;
            cout << "[3] Algoritmo construtivo guloso randomizado reativo" << endl;
            int selecao_sub;
            cin >> selecao_sub;
            if (selecao_sub == 1) {
                graph->greedyConstructiveAlgorithm(output_file);
            } else if (selecao_sub == 2) {
                float alpha;
                int qtd_iteracoes;
                cout << "Valor de alfa: ";
                cin >> alpha;
                cout << "Quantidade de iterações: ";
                cin >> qtd_iteracoes;
                graph->adaptiveRandomizedGreedyAlgorithm(alpha, qtd_iteracoes, output_file,
                                                         true);
            } else if (selecao_sub == 3) {
                vector<float> alphas = {0.05, 0.10, 0.15, 0.30, 0.50};
                int qtd_iteracoes;
                int tam_bloco;
                cout << "Quantidade de iterações: ";
                cin >> qtd_iteracoes;
                cout << "Tamanho do bloco: ";
                cin >> tam_bloco;
                graph->reactiveRandomizedGreedyConstructiveAlgorithm(alphas, qtd_iteracoes,
                                                                     tam_bloco, 5, output_file);
            }
            break;
        }

    }
}

int mainMenu(ofstream &output_file, Graph *graph) {

    int selecao = 1;

    while (selecao != 0) {

        selecao = menu();
        system("clear");
        if (output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Unable to open the output_file" << endl;

        output_file << endl;

    }

    return 0;
}

int main(int argc, char const *argv[]) {

    //Verificação se todos os parâmetros do programa foram entrados
    if (argc != 6 && argc != 3) {

        cout
                << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> "
                << endl;
        return 1;

    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;


    //Abrindo arquivo de entrada
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    output_file << argv[1] << endl;

    Graph *graph;

    if (input_file.is_open()) {
        if (argc == 3)
            graph = leituraInstanciaSubconjuntoDominantePonderado(input_file, 0, 0, 1);
        else
            graph = leitura(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    } else
        cout << "Unable to open " << argv[1];

//    graph->greedyConstructiveAlgorithm(output_file);

//    vector<float> alphas1 = {0.15, 0.30, 0.50};
//    for (int i = 0; i < alphas1.size(); i++) {
//        for (int j = 0; j < 10; j++) {
//            solucao *s = graph->adaptiveRandomizedGreedyAlgorithm(alphas1[i], 1000, output_file, true);
//            delete s;
//        }
//    }

//    vector<float> alphas2 = {0.05, 0.10, 0.15, 0.30, 0.50};
//    int iteracoes = 3500;
//    int tam_bloco = 250;
//    for (int i = 0; i < 10; i++) {
//        graph->reactiveRandomizedGreedyConstructiveAlgorithm(alphas2, iteracoes,
//                                                             tam_bloco, 5, output_file);
//
//    }

    mainMenu(output_file, graph);

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}

