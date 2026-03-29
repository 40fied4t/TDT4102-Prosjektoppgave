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
                updateMain();
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

void GraphWindow::updateMain() {
    TDT4102::Point currMouseLocation = get_mouse_coordinates();
    TDT4102::Point deltaMouseMovement = currMouseLocation - lastMouseLocation;

    if (framesSinceLastLeftClick < 100)
        framesSinceLastLeftClick++;
    
    if (std::shared_ptr<Node> hoveredNode{getNode(currMouseLocation)}) {


        if (is_left_mouse_button_down() && framesSinceLastLeftClick < 20) {
            if (is_key_down(KeyboardKey::LEFT_CTRL) || is_key_down(KeyboardKey::RIGHT_CTRL)) {
                addNode(get_mouse_coordinates(), nextLabel);
                updateNextLabel();
            }
            else {

            }

            framesSinceLastLeftClick = 0;
        }
        while (is_left_mouse_button_down()) {
            hoveredNode -> setLocation(hoveredNode -> getLocation() + deltaMouseMovement);
            drawMain();
            next_frame();

            lastMouseLocation = currMouseLocation;
            currMouseLocation = get_mouse_coordinates();
            deltaMouseMovement = currMouseLocation - lastMouseLocation;

            framesSinceLastLeftClick = 0;
        }


        if (is_right_mouse_button_down() && !rightMouseButtonClick) {
            hoveredNode -> updateSelect();
            rightMouseButtonClick = true;
        }
        else if (!is_right_mouse_button_down()) {
            rightMouseButtonClick = false;
        }
    }

    lastMouseLocation = currMouseLocation;
}

void GraphWindow::drawMain() {
    drawAllEdges();
    drawAllNodes();
}

void GraphWindow::drawMenu() {
}


void GraphWindow::drawNode(const std::shared_ptr<Node>& node){ 
    draw_circle(node -> getLocation(), radius, node -> isSelected() ? selectedNodeColor : unselectedNodeColor,borderColor);
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

std::shared_ptr<Node> GraphWindow::getNode(const TDT4102::Point& location) const {
    auto p = find_if(
        nodes.begin(),
        nodes.end(),
        [&] (std::shared_ptr<Node> const& p) {
            return (getDistance(p -> getLocation(), get_mouse_coordinates())) <= radius;
        }
    );

    if (p==nodes.end()) {
        return nullptr;
    }
    return *p;
}