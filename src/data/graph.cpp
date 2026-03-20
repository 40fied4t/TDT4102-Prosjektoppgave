#include "graph.h"
//implementer Node
Node::Node(TDT4102::Point location, std::string label):
    label{label},
    location{location}
{}

TDT4102::Point Node::getLocation() const {
    return location;
}

std::string Node::getLabel() const {
    return label;
}

bool Node::isSelected() const {
    return selected;
}

void Node::updateSelect() {
    selected = !selected;
}

//implementer Edge, og arvede klasser
Edge::Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int weight):
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

DirectionalEdge::DirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, int weight):
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
void Graph::addEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, const int weight){
    bool alreadyTo = false;
    bool alreadyFrom = false;
    for (auto &it : graphMap[from]) {
        if (it == to){
            alreadyFrom = true;
        }
    }
    for (auto &it : graphMap[to]) {
        if (it == from){
            alreadyTo = true;
        }
    }
    if (alreadyFrom || alreadyFrom) {return;}
    
    graphMap[from].push_back(to);
    graphMap[to].push_back(from);
    edgeVec.emplace_back(std::make_unique<Edge>(from, to, weight));
}

void Graph::addDirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, const int weight) {
    if (from == to) {return;}
    bool alreadyTo = false;
    bool alreadyFrom = false;
    for (auto &it : graphMap[from]) {
        if (it == to){
            alreadyFrom = true;
        }
    }
    for (auto &it : graphMap[to]) {
        if (it == from){
            alreadyTo = true;
        }
    }
    if (alreadyFrom || alreadyTo) {return;}

    graphMap[from].push_back(to);
    edgeVec.emplace_back(std::make_unique<DirectionalEdge>(from, to, weight));
}

void Graph::addSelectedEdges(const int weight){
    updateSelectedNodes();

    for (auto i = selectedNodes.begin(); i != selectedNodes.end(); ++i) {
        for (auto j = i; j != selectedNodes.end(); ++j) {
            addEdge(*i, *j, weight);
        }
    }
}

void Graph::removeEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to) {}
void Graph::removeSelectedEdges(){}

void Graph::removeAllEdgesToNode(std::shared_ptr<Node>){}
void Graph::addNode(const TDT4102::Point location, const int& label){}
void Graph::removeNode(){}
void Graph::removeSelectedNodes(){}
int Graph::getSize() const {
    return nodes.size();
}
int Graph::getEdgeNum() const {
    return edgeVec.size();
}

void Graph::empty() {
    nextLabel = 1;
    graphMap.clear();
    edgeVec.clear();
    nodes.clear();
    selectedNodes.clear();
}

Graph::Graph():
    graphMap{},
    edgeVec{},
    nodes{},
    selectedNodes{}
{
    addNode({width/2, height/2}, nextLabel);
    updateNextLabel();
}
Graph::Graph(std::filesystem::path fileName){}
Graph::~Graph(){}

void Graph::loadFromAdj(std::filesystem::path fileName){
    //Resetter graf
    empty();

    std::ifstream inputStream{fileName};
    try {
        if (!inputStream) {
            throw FileNotOpen{fileName};
        }
        if (!std::filesystem::equivalent(fileName.extension(), std::filesystem::path(".adj"))) {
            throw WrongFileExtension{fileName, std::filesystem::path(".adj")};
        }
    }
    catch (FileNotOpen e){
        std::cerr << e.what() << " " << e.getPath() << std::endl;
        return;
    }
    catch (WrongFileExtension e) {
        std::cerr << e.what() << "\nforventet: " << e.getExtension() << "\fikk: " << e.getPath() << std::endl;
        return;
    }

    //
    std::string key;
    std::string val;
    char delim;
    std::unordered_map<std::string, std::vector<std::string>> labelMap;
    std::vector<std::string> labelVec;
    while (inputStream) {
        inputStream >> key;
        inputStream >> delim;

        if (delim != ':') {
            throw BadFormat();
            return;
        }
        while (inputStream >> val) {
            if (val == "\n") {break;}

            labelMap[key].push_back(val);
        }
    }
}
void Graph::loadFromEdg(std::filesystem::path fileName){
    
}

void Graph::saveToAdj(std::filesystem::path fileName){

}
void Graph::saveToEdg(std::filesystem::path fileName){
    graphMap.clear();
    edgeVec.clear();
    nodes.clear();
    selectedNodes.clear();
}