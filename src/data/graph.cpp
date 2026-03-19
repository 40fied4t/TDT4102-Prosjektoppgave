#include "graph.h"
//implementer Node
Node::Node(TDT4102::Point location, const int& label):
    label{label},
    location{location}
{}
TDT4102::Point Node::getLocation() const {
    return location;
}
int Node::getLabel() const {
    return label;
}
bool Node::isSelected() const {
    return selected;
}
void Node::updateSelect() {
    selected = !selected;
}


//implementer Graph
void Graph::updateSelectedNodes(){
    for (auto it = nodes.begin(); it != nodes.end();++it) {

    }
}
void Graph::addEdges(){}
void Graph::removeEdges(){}
void Graph::addNode(const TDT4102::Point location, const int& label){}
void Graph::removeNode(){}
void Graph::removeSelectedNodes(){}
int Graph::getSize(){}
int Graph::getEdgeNum(){}


Graph::Graph(){
    addNode({width/2, height/2}, nextLabel);
    updateNextLabel();
}
Graph::Graph(std::filesystem::path fileName){}
Graph::~Graph(){}

void Graph::loadFromAdj(std::filesystem::path fileName){}
void Graph::loadFromEdg(std::filesystem::path fileName){}

void Graph::saveToAdj(std::filesystem::path fileName){}
void Graph::saveToEdg(std::filesystem::path fileName){}