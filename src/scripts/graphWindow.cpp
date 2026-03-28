#include "include/graphWindow.h"

GraphWindow::GraphWindow():
    Graph(),
    AnimationWindow(50, 50, Graph::width, Graph::height, "Grafvisualisering"),


    loadButton{TDT4102::Point{bufW,50}, btnW, btnH, "load"},
    saveButton{TDT4102::Point{bufW, 100}, btnW, btnH, "save"},
    changeMenuButton{TDT4102::Point{Graph::width - bufW - btnW, bufH}, btnW, btnH, "change"},

    fileInput{TDT4102::Point{bufW, 150}, fieldW, fieldH, "filename"}
    {
    loadButton.setCallback(std::bind(&GraphWindow::loadButtonCallback, this));
    saveButton.setCallback(std::bind(&GraphWindow::saveButtonCallback, this));
    changeMenuButton.setCallback(std::bind(&GraphWindow::changeMenuCallback, this));

    add(loadButton);
    add(saveButton);
    add(changeMenuButton);
    add(fileInput);
}

GraphWindow::GraphWindow(std::filesystem::path fileName):
    Graph(fileName),
    AnimationWindow(50, 50, Graph::width, Graph::height, "Grafvisualisering"),

    loadButton{TDT4102::Point{bufW,100}, btnW, btnH, "load"},
    saveButton{TDT4102::Point{bufW, 200}, btnW, btnH, "save"},
    changeMenuButton{TDT4102::Point{Graph::width - bufW - btnW, bufH}, btnW, btnH, "change"},

    fileInput{TDT4102::Point{bufW, 300}, fieldW, fieldH, "filename"}
    {
    loadButton.setCallback(std::bind(&GraphWindow::loadButtonCallback, this));
    saveButton.setCallback(std::bind(&GraphWindow::saveButtonCallback, this));
    changeMenuButton.setCallback(std::bind(&GraphWindow::changeMenuCallback, this));

    add(loadButton);
    add(saveButton);
    add(changeMenuButton);
    add(fileInput);
}

void GraphWindow::run() {
    while(!should_close()){
        switch (currState) {
            case menu:
                loadButton.setVisible(true);
                saveButton.setVisible(true);
                fileInput.setVisible(true);
                drawMenu();
                next_frame();
                break;
            case main:
                loadButton.setVisible(false);
                saveButton.setVisible(false);
                fileInput.setVisible(false);
                drawMain();
                next_frame();
                break;
            case closed:
                close();
                break;
            default:
                std::cerr << "wrong Currstate\n";
                close();
        }
    }
}

void GraphWindow::drawMain() {
    drawAllEdges();
    drawAllNodes();
}

void GraphWindow::drawMenu() {
}

std::shared_ptr<Node>& GraphWindow::getNode(const TDT4102::Point& location) {
    return *std::find_if(
        nodes.begin(),
        nodes.end(),
        [&] (std::shared_ptr<Node> const& p) {
            return (getDistance(p -> getLocation(), get_mouse_coordinates())) <= radius;
        }
    );
}

double GraphWindow::getDistance(const TDT4102::Point& loc1, const TDT4102::Point& loc2){
    int diffX = loc1.x - loc2.x;
    int diffY = loc1.y - loc2.y;

    return std::sqrt(diffX * diffX + diffY * diffY);
}

void GraphWindow::drawNode(const std::shared_ptr<Node>& node){ 
    draw_circle(node -> getLocation(), radius, nodeColor, node -> isSelected() ? selectedBorderColor : unselectedBorderColor);
}
void GraphWindow::drawEdge(const TDT4102::Point& startPoint, const TDT4102::Point& endPoint, const bool selected){
    draw_line(startPoint, endPoint, selected ? selectedEdgeColor : unselectedEdgeColor);
}

void GraphWindow::drawAllNodes(){
    for (const auto& it : nodes) {
        drawNode(it);
    }
}
void GraphWindow::drawAllEdges(){
    for (const auto& it : edgeVec) {
        for (const auto& from : it -> getFrom()) {
            for (const auto& to : it -> getTo()) {
                if (from != to) {
                    drawEdge(from -> getLocation(), to -> getLocation(), it -> getSelect());
                }
            }
        }
    }
}


void GraphWindow::loadButtonCallback(){
    std::filesystem::path fileName = fileInput.getText();


    if (fileName.extension() == ".adj") {
        loadFromAdj(fileName);
    }
    else if (fileName.extension() == ".edg") {
        loadFromEdg(fileName);
    }
    else {
        /*Kanskje vis en melding?*/
    }
}
void GraphWindow::saveButtonCallback(){
    std::filesystem::path fileName = fileInput.getText();

    // if (!fileName.has_extension()) {
    //     fileName.replace_extension(".adj");
    //     saveToAdj(fileName);
    //     fileName.replace_extension(".edg");
    //     saveToEdg(fileName);
    // }
    if (fileName.extension() == ".adj") {
        saveToAdj(fileName);
    }
    else if (fileName.extension() == ".edg") {
        saveToEdg(fileName);
    }
    else {
        /*Kanskje vis melding?*/
    }
}

void GraphWindow::changeMenuCallback() {
    switch (currState) {
        case main:
            currState = menu;
            break;
        case menu:
            currState = main;
            break;
    }
}