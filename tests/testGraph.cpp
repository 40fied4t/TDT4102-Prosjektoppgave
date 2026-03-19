#include "testGraph.h"

void TestGraph::testLoad() {
    Graph g1;
    Graph g2("tests/adjecencylist.adj");
    Graph g3("tests/edgeList.edg");

    bool passed = true;

    if (g1.getSize() != 1 && g1.getEdgeNum() != 0) {
        passed = false;
        std::cout << "testLoad failed\nfeil i konstruktør Graph(), forventet getSize 1, og getEdgeNum 0 men fikk" << g1.getSize()
        << " og " << g1.getEdgeNum() << std::endl;
    }
    if (g2.getSize() != 1 && g2.getEdgeNum() != 0) {
        passed = false;
        std::cout << "testLoad failed\nfeil i konstruktør Graph(), forventet getSize 4, og getEdgeNum 8 men fikk" << g2.getSize()
        << " og " << g2.getEdgeNum() << std::endl;
    }
    if (g3.getSize() != 1 && g3.getEdgeNum() != 0) {
        passed = false;
        std::cout << "testLoad failed\nfeil i konstruktør Graph(), forventet getSize 4, og getEdgeNum 8 men fikk" << g3.getSize()
        << " og " << g3.getEdgeNum() << std::endl;
    }
    if (passed) {
        std::cout << "testLoad passed\n";
    }

}

void TestGraph::testSave(){
    Graph g1("tests/adjecencylist.adj");
    Graph g2("tests/edgeList.edg");

    g1.saveToAdj("tests/output/adjecencylist.adj");
    g2.saveToEdg("tests/output/edgeList.edg");

    std::cout << "testSave kjørt, sjekk output\n";
}
void TestGraph::testAddNode(){
    Graph g;
    g.addNode({500, 500}, 3);
}
