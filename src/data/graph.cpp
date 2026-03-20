#include "graph.h"
//implementer Node
Node::Node(TDT4102::Point location, const int label):
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

//implementer Edge, og arvede klasser
Edge::Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, const int weight):
    nodeVec{from, to},
    weight{weight}
{}

Edge::Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to):
    nodeVec{from, to},
    weight{1}
{}

std::vector<std::shared_ptr<Node>> Edge::getFrom() const {
    return nodeVec;
}

DirectionalEdge::DirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, const int weight):
    Edge{from, to, weight}
{}

DirectionalEdge::DirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to):
    Edge{from, to}
{}

std::vector<std::shared_ptr<Node>> DirectionalEdge::getFrom() const {
    std::vector<std::shared_ptr<Node>> fromVec{nodeVec[0]};
    return fromVec;
}

std::vector<std::shared_ptr<Node>> DirectionalEdge::getTo() const {
    std::vector<std::shared_ptr<Node>> toVec{nodeVec[1]};
    return toVec;
}

//implementer Graph
void Graph::updateSelectedNodes(){
    selectedNodes.clear();
    for (auto &it : nodes) {
        if (it -> isSelected()) {
            selectedNodes.emplace_back(*it);
        }
    }
}

void Graph::updateNextLabel() {
    nextLabel = 1;
    for (auto &it : nodes) {
        if (nextLabel <= it -> getLabel()) {
            nextLabel = it -> getLabel() + 1;
        }
    }
}
void Graph::addEdge(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
    if (node1 == node2) {
        return;
    }
    bool alreadyInVec = false;
    for (auto &it : graphMap[node1]) {
        if (it == node2) {alreadyInVec = true;}
    }
    if (!alreadyInVec) {
            graphMap[node1].push_back(node2);
            graphMap[node2].push_back(node1);
            edgeVec.push_back({node1, node2});
    }
}
void Graph::addSelectedEdges(){
    updateSelectedNodes();
    for (auto i = selectedNodes.begin(); i != selectedNodes.end(); ++i) {
        for (auto j = i + 1; j != selectedNodes.end(); ++j) {
            addEdge(*i, *j);
        }
    }
}
void Graph::removeEdge(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {

}
void Graph::removeSelectedEdges(){}
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