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

    if (framesSinceLastHoveredLeftClick < 100)
        framesSinceLastHoveredLeftClick++;

    //================================================== Node Hovered
    if (std::shared_ptr<Node> hoveredNode{getNode(currMouseLocation)}) {

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ LCLICK
        
        if (is_left_mouse_button_down() && framesSinceLastHoveredLeftClick < 10) {

            auto newNode = addNode(get_mouse_coordinates(), nextLabel);
            int w = getInputWeight();
            updateNextLabel();

            if (is_key_down(KeyboardKey::LEFT_CTRL) || is_key_down(KeyboardKey::RIGHT_CTRL)) {
                addDirectionalEdge(hoveredNode, newNode, w);
            }
            else {
                addEdge(hoveredNode, newNode, w);
            }
            std::swap(hoveredNode, newNode);
            framesSinceLastHoveredLeftClick = 0;
        }
        while (is_left_mouse_button_down()) {
            hoveredNode -> setLocation(hoveredNode -> getLocation() + deltaMouseMovement);
            drawMain();
            next_frame();

            lastMouseLocation = currMouseLocation;
            currMouseLocation = get_mouse_coordinates();
            deltaMouseMovement = currMouseLocation - lastMouseLocation;

            framesSinceLastHoveredLeftClick = 0;
        }

        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ RCLICK
        if (is_right_mouse_button_down() && !rightMouseButtonClick) {
            hoveredNode -> updateSelect();
            rightMouseButtonClick = true;
        }
        else if (!is_right_mouse_button_down()) {
            rightMouseButtonClick = false;
        }
    }
    //================================================== DRAG

    while (is_left_mouse_button_down()) {
        for (auto & it : nodes) {
            it -> setLocation(it -> getLocation() + 0.8 * deltaMouseMovement);
        }
            
        drawMain();
        next_frame();

        lastMouseLocation = currMouseLocation;
        currMouseLocation = get_mouse_coordinates();
        deltaMouseMovement = currMouseLocation - lastMouseLocation;
    }


    //================================================== ZOOM

    if (float deltaMouseWheel = get_delta_mouse_wheel()) {
        TDT4102::Point mouseCoords = get_mouse_coordinates();
        for (auto& it : nodes) {
            TDT4102::Point delta = 0.1 * deltaMouseWheel * (it -> getLocation() - mouseCoords);
            it -> setLocation(it -> getLocation() + delta);
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
    TDT4102::Point tangent = getTangent(endPoint - startPoint);
    TDT4102::Point normal = getNormal(endPoint - startPoint);

    draw_quad(
        startPoint + (edgeW * normal),
        endPoint + (edgeW * normal),
        endPoint - (edgeW * normal),
        startPoint - (edgeW * normal),
        selected ? selectedEdgeColor : unselectedEdgeColor
    );
    draw_triangle(
        endPoint -  0.1 * radius * tangent,
        endPoint - 0.5 * edgeW * radius * tangent +  5 * edgeW * normal,
        endPoint - 0.5 * edgeW * radius * tangent - 5 * edgeW * normal,
        selected ? selectedEdgeColor : unselectedEdgeColor
    );
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

    if (!fileName.has_extension()) {
        fileName.replace_extension(".adj");
        saveToAdj(fileName);
        fileName.replace_extension(".edg");
        saveToEdg(fileName);
    }
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

int GraphWindow::getInputWeight() const {
    if (is_key_down(KeyboardKey::KEY_1) || is_key_down(KeyboardKey::NUMPAD_1)) 
        return 1;
    else if (is_key_down(KeyboardKey::KEY_2) || is_key_down(KeyboardKey::NUMPAD_2))
        return 2;
    else if (is_key_down(KeyboardKey::KEY_3) || is_key_down(KeyboardKey::NUMPAD_3))
        return 3;
    else if (is_key_down(KeyboardKey::KEY_4) || is_key_down(KeyboardKey::NUMPAD_4))
        return 4;
    else if (is_key_down(KeyboardKey::KEY_5) || is_key_down(KeyboardKey::NUMPAD_5))
        return 5;
    else if (is_key_down(KeyboardKey::KEY_6) || is_key_down(KeyboardKey::NUMPAD_6)) 
        return 6;
    else if (is_key_down(KeyboardKey::KEY_7) || is_key_down(KeyboardKey::NUMPAD_7))
        return 7;
    else if (is_key_down(KeyboardKey::KEY_8) || is_key_down(KeyboardKey::NUMPAD_8))
        return 8;
    else if (is_key_down(KeyboardKey::KEY_9) || is_key_down(KeyboardKey::NUMPAD_9))
        return 9;
    else if (is_key_down(KeyboardKey::KEY_0) || is_key_down(KeyboardKey::NUMPAD_0))
        return 0;
    return 1;
}