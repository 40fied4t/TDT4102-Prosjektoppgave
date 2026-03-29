#include "include/graph.h"



//implementer Edge, og arvede klasser

//implementer Graph
void Graph::updateSelectedNodes(){
    selectedNodes.clear();
    for (auto &it : nodes) {
        if (it -> isSelected()) {
            selectedNodes.emplace_back(it);
        }
    }
}
void Graph::updateNextLabel(){
    int labelInt = 1;
    auto it = labelVec.begin();
    while (it != labelVec.end()) {
        if (*it == std::to_string(labelInt)) {
            labelInt++;
            it = labelVec.begin();
        }
        else {
            it++;
        }
    }
    nextLabel = std::to_string(labelInt);
}

void Graph::resetLabels() {
    labelVec.clear();
    std::string newLabel;
    nextLabel = "1";
    for (int i = 0; i < getSize(); i++) {
        newLabel = nextLabel;
        nextLabel = std::to_string(i + 2);
        nodes[i] -> setLabel(newLabel);
        labelVec.push_back(newLabel);
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
    bool alreadyFromFromToTo = leadsTo<std::shared_ptr<Node>>(graphMap, from, to);
    bool alreadyFromToToFrom = leadsTo<std::shared_ptr<Node>>(graphMap, to, from);
    if (alreadyFromFromToTo || alreadyFromToToFrom) {return;}

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

void Graph::removeEdge(std::shared_ptr<Node> first, std::shared_ptr<Node> second) {
    //Fjerner kanten fra edgeVec
    if (first == second) return;
    bool firstFound;
    bool secondFound;
    for (auto& it : edgeVec) {
        firstFound = false;
        secondFound = false;
        for (auto nodeIterator : it -> getNodeVec()) {
            if (nodeIterator == first) {
                firstFound = true;
            }
            if (nodeIterator == second) {
                secondFound = true;
            }
        }
        if (firstFound && secondFound) {
            edgeVec.erase(
                std::remove_if( // Fjern elementet fra edgeVec
                    edgeVec.begin(),
                    edgeVec.end(),
                    //Benytter lambdafunksjon som predikat
                    [&] (std::shared_ptr<Edge> const& p)
                        {   // Predikatet sjekker om de peker til samme underliggende Edge
                        return p == it;
                    }),
                edgeVec.end()
                );
            //Fjerner kanten fra graphMap
            for (auto& fromIt : it -> getFrom()) {
                graphMap[fromIt].erase(
                    std::remove_if( // Fjerner element fra graphMap[fromIt] dersom det ligger i toIt
                        graphMap[fromIt].begin(),
                        graphMap[fromIt].end(),
                        //Benytter lambdafunksjon som predikat
                        [&] (std::shared_ptr<Node> const& p)
                            {
                            for (auto& toIt : it -> getTo()) {
                                if (p == toIt) return true;
                            }
                            return false;
                        }),
                    graphMap[fromIt].end()
                    );
            }
            //Fant kanten
            return;
        }
    }
    // Fant ikke kanten
    return;
}

void Graph::removeSelectedEdges(){
    updateSelectedNodes();

    for (auto i = selectedNodes.begin(); i != selectedNodes.end();++i) {
        for (auto j = i + 1; j != selectedNodes.end();++j) {
            removeEdge(*i, *j);
        }
    }
}

void Graph::removeAllEdgesOfNode(std::shared_ptr<Node> node){
    for (auto& it : nodes) {
        removeEdge(node, it);
    }
}
std::shared_ptr<Node> Graph::addNode(const TDT4102::Point location, std::string label){
    nodes.emplace_back(std::make_shared<Node>(location, label));
    return nodes.back();
}
void Graph::removeNode(const std::shared_ptr<Node> node){
    
    removeAllEdgesOfNode(node);

    graphMap.erase(node);

    nodes.erase(
        std::remove(
            nodes.begin(),
            nodes.end(),
            node),
        nodes.end()
        );

    labelVec.erase(
        std::remove(
            labelVec.begin(),
            labelVec.end(),
            node -> getLabel()),
        labelVec.end()
        );

    selectedNodes.erase(
        std::remove(
            selectedNodes.begin(),
            selectedNodes.end(),
            node),
        selectedNodes.end()
        );

    updateNextLabel();

    if (node.use_count() > 1){
        std::cerr << "Did not remove all instances of node\n";
    }
}
void Graph::removeSelectedNodes(){
    updateSelectedNodes();

    while (std::shared_ptr<Node> node = selectedNodes.back()) {
        selectedNodes.pop_back();
        removeNode(node);
    }
}
int Graph::getSize() const {
    return nodes.size();
}
int Graph::getEdgeNum() const {
    return edgeVec.size();
}

std::shared_ptr<Node> Graph::getNode(const std::string& label) const {
    for (auto &it : nodes) {
        if (it -> getLabel() == label) {
            return it;
        }
    }
    std::cerr << "Node not found!" << std::endl;
    return nullptr;
}
void Graph::empty() {
    nextLabel = 1;
    graphMap.clear();
    edgeVec.clear();
    nodes.clear();
    selectedNodes.clear();
}

std::vector<TDT4102::Point> Graph::generatePositions(const int& n) {
    int d = width/(2*n);
    std::vector<TDT4102::Point> positionVec;
    positionVec.reserve(n);
    for (int i = 0; i < n; ++i) {
        positionVec.push_back({d * (2*i + 1), height/2});
    }
    return positionVec;
}

Graph::Graph()
{
    graphMap = std::unordered_map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>>();
    edgeVec = std::vector<std::shared_ptr<Edge>>();
    nodes = std::vector<std::shared_ptr<Node>>();
    selectedNodes = std::vector<std::shared_ptr<Node>>();
    addNode({width/2, height/2}, nextLabel);
    updateNextLabel();
}
Graph::Graph(std::filesystem::path fileName)
{
    graphMap = std::unordered_map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>>();
    edgeVec = std::vector<std::shared_ptr<Edge>>();
    nodes = std::vector<std::shared_ptr<Node>>();
    selectedNodes = std::vector<std::shared_ptr<Node>>();

    if (fileName.extension() == ".adj") {
        std::cout << "Loading .adj file\n";
        loadFromAdj(fileName);

    }
    else if (fileName.extension() == ".edg") {
        std::cout << "Loading .edg file\n";
        loadFromEdg(fileName);
    }
    else {
        std::cerr << "File does not have correct format extension";
    }
}
Graph::~Graph(){
    empty();
    labelVec.clear();
}

void Graph::loadFromAdj(std::filesystem::path fileName){
    std::ifstream inputStream{fileName};
    try {
        if (!inputStream) {
            throw FileNotOpen{fileName};
        }
        if (fileName.extension() != std::filesystem::path(".adj")) {
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

    //generer referansestrukturer for generering av graf
    labelVec.clear();
    std::string key;
    std::string val;
    std::string line;

    char delim;
    std::unordered_map<std::string, std::vector<std::string>> labelMap;
    try{
    while (inputStream >> key) {
        inputStream >> delim;

        for (auto &it : labelVec) {
            if (it == key) {
                std::cerr << "Already in vector\n";
                throw BadFormat();
            }
        }
        labelVec.push_back(key);

        if (delim != ':') {
            std::cerr << "dårlig delim\n";
            throw BadFormat();
        }
        std::getline(inputStream, line);

        std::stringstream ss{line};
        while (ss >> val) {
            labelMap[key].push_back(val);
        }
    }
    } catch (BadFormat e) {
        std::cout << e.what();
        return;
    }

    std::vector<TDT4102::Point> positionVec = generatePositions(labelVec.size());
    //Avbryt dersom 
    if (labelMap.size() != positionVec.size() || positionVec.size() != labelVec.size()) {
        std::cout << "size error" << labelMap.size() << " " << positionVec.size() << " " << labelVec.size() << std::endl;
        throw BadFormat();
        return;
    }
    //tøm graf og generer nye noder
    empty();
    for (int i = 0; i < labelVec.size(); ++i) {
        addNode(positionVec[i], labelVec[i]);
    }
    updateNextLabel();
    //generer kanter
    for (auto i = labelVec.begin(); i != labelVec.end(); ++i) {
        for (auto j = i +1; j != labelVec.end(); ++j) {
            bool iToj = leadsTo<std::string>(labelMap, *i, *j);
            bool jToi = leadsTo<std::string>(labelMap, *j, *i);

            if (iToj && jToi) {
                addEdge(getNode(*i), getNode(*j), 1);
            }
            else if (iToj) {
                addDirectionalEdge(getNode(*i), getNode(*j), 1);
            }
            else if (jToi) {
                addDirectionalEdge(getNode(*j), getNode(*i), 1);
            }
        }
    }
}
void Graph::loadFromEdg(std::filesystem::path fileName){
    std::ifstream inputStream{fileName};
    try {
        if (!inputStream) {
            throw FileNotOpen{fileName};
        }
        if (fileName.extension() != std::filesystem::path(".edg")) {
            throw WrongFileExtension{fileName, std::filesystem::path(".edg")};
        }
    }
    catch (FileNotOpen e){
        std::cerr << e.what() << " " << e.getPath() << std::endl;
        return;
    }
    catch (WrongFileExtension e) {
        std::cerr << e.what() << "\nforventet: " << e.getExtension() << "\nfikk: " << e.getPath() << std::endl;
        return;
    }

    std::string firstLabel;
    char edge;
    std::string secondLabel;
    int weight;

    struct LabelEdge {
        std::string firstLabel;
        std::string secondLabel;
        int edge;
        int weight;
    };
    std::vector<LabelEdge> labelEdgeVec;
    labelVec.clear();

    while (inputStream >> firstLabel >> edge >> secondLabel >> weight) {
        if (!(edge == '-' || edge == '<' || edge == '>')) {
            std::cout << edge << std::endl;
            throw BadFormat();
            return;
        }
        if (std::find(labelVec.begin(), labelVec.end(), firstLabel) == labelVec.end()){  
            labelVec.push_back(firstLabel);
        }
        else if (std::find(labelVec.begin(), labelVec.end(), secondLabel) == labelVec.end()){  
            labelVec.push_back(secondLabel);
        }
        labelEdgeVec.emplace_back(LabelEdge{firstLabel, secondLabel, edge, weight});
    }

    std::vector<TDT4102::Point> positionVec = generatePositions(labelVec.size());
    //Avbryt dersom 
    if (positionVec.size() != labelVec.size()) {
        std::cout << "size error " << positionVec.size() << " " << labelVec.size() << std::endl;
        throw BadFormat();
        return;
    }
    //tøm graf og generer nye noder
    empty();
    for (int i = 0; i < labelVec.size(); ++i) {
        addNode(positionVec[i], labelVec[i]);
    }
    updateNextLabel();
    //generer kanter
    for (auto& it : labelEdgeVec) {
        if (it.edge == '-') {
            addEdge(getNode(it.firstLabel), getNode(it.secondLabel), it.weight);
        }
        else if (it.edge == '>') {
            addDirectionalEdge(getNode(it.firstLabel), getNode(it.secondLabel), it.weight);
        }
        else if (it.edge == '<') {
            addDirectionalEdge(getNode(it.secondLabel), getNode(it.firstLabel), it.weight);
        }
    }
}

void Graph::saveToAdj(std::filesystem::path fileName){
    std::ofstream outputStream{fileName};

    for (auto& it : graphMap) {
        std::shared_ptr<Node> keyNode = std::get<0>(it);
        std::vector<std::shared_ptr<Node>> valNodeVec = std::get<1>(it);

        outputStream << keyNode -> getLabel() << " :";
        for (auto& valNode : valNodeVec) {
            outputStream << " " << valNode -> getLabel();
        }
        outputStream << "\n";
    }
}
void Graph::saveToEdg(std::filesystem::path fileName){
    std::ofstream outputStream{fileName};

    for (auto& it : edgeVec) {
        //hvis Edge
        if (it -> getDelim() == '-') {
            outputStream << 
            it -> getFrom()[0] -> getLabel() << " - " <<
            it -> getFrom()[1] -> getLabel() << " " <<
            it -> getWeight() << "\n";
        }
        //hvis DirectionalEdge
        else if (it -> getDelim() == '>') {
            outputStream << 
            it -> getFrom()[0] -> getLabel() << " > " <<
            it -> getTo()[0] -> getLabel() << " " <<
            it -> getWeight() << "\n";
        }
    }
}


double Graph::getDistance(const TDT4102::Point& first, const TDT4102::Point& second) const {
    int diffX = first.x - second.x;
    int diffY = first.y - second.y;

    return std::sqrt(diffX * diffX + diffY * diffY);
}