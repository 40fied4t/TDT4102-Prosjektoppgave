#include "include/graphWindow.h"

GraphWindow::GraphWindow():
    Graph(),
    AnimationWindow(50, 50, Graph::width, Graph::height, "Grafvisualisering"),


    loadButton{TDT4102::Point{100,100}, btnW, btnH, "load"},
    saveButton{TDT4102::Point{100, 200}, btnW, btnH, "save"},

    fileInput{TDT4102::Point{100, 300}, fieldW, fieldH, "filename"}
    {
    loadButton.setCallback(std::bind(&GraphWindow::loadButtonCallback, this));
    saveButton.setCallback(std::bind(&GraphWindow::saveButtonCallback, this));

    add(loadButton);
    add(saveButton);
}

GraphWindow::GraphWindow(std::filesystem::path fileName):
    Graph(fileName),
    AnimationWindow(50, 50, Graph::width, Graph::height, "Grafvisualisering"),

    loadButton{TDT4102::Point{100,100}, btnW, btnH, "load"},
    saveButton{TDT4102::Point{100, 200}, btnW, btnH, "save"},

    fileInput{TDT4102::Point{100, 300}, fieldW, fieldH, "filename"}
    {
    loadButton.setCallback(std::bind(&GraphWindow::loadButtonCallback, this));
    saveButton.setCallback(std::bind(&GraphWindow::saveButtonCallback, this));

    add(loadButton);
    add(saveButton);
}

void GraphWindow::run() {
    while(!should_close()){
        switch (currState) {
            case menu:
                break;
            case main:
                break;
            case closed:
                close();
                break;
            default:
                std::cerr << "wrong Currstate";
                close();
        }
    }
}

void GraphWindow::drawMain() {
    
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
void GraphWindow::drawAllEdges(){}


void GraphWindow::loadButtonCallback(){}
void GraphWindow::saveButtonCallback(){}