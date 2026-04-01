#include "include/graphWindow.h"

// GraphWindow::GraphWindow():
//     Graph(),
//     AnimationWindow(0, 0, Graph::width, Graph::height, "Grafvisualisering"),


//     loadButton{TDT4102::Point{bufW,50}, btnW, btnH, "load"},
//     saveButton{TDT4102::Point{bufW, 100}, btnW, btnH, "save"},
//     pauseMenuButton{TDT4102::Point{Graph::width - bufW - btnW, bufH}, btnW, btnH, "pause"},

//     fileInput{TDT4102::Point{bufW, 150}, fieldW, fieldH, "filename"}
//     {
//     loadButton.setCallback(std::bind(&GraphWindow::loadButtonCallback, this));
//     saveButton.setCallback(std::bind(&GraphWindow::saveButtonCallback, this));
//     pauseMenuButton.setCallback(std::bind(&GraphWindow::pauseMenuCallback, this));

//     add(loadButton);
//     add(saveButton);
//     add(pauseMenuButton);
//     add(fileInput);
// }

GraphWindow::GraphWindow(std::filesystem::path fileName):
    Graph(fileName),
    AnimationWindow(50, 50, 1024, 768, "Grafvisualisering"),

    //========================= Buttons

    importMenuButton{mainMenuTopLeft + buffer + 2 * bufferH, btnW, btnH, "import"},
    exportMenuButton{mainMenuTopLeft + buffer + 3 * bufferH, btnW, btnH, "export"},
    infoMenuButton{mainMenuTopLeft + buffer + 4 * bufferH, btnW, btnH, "info"},
    colorMenuButton{mainMenuTopLeft + buffer + 5 * bufferH, btnW, btnH, "color"}, // Maybe dropdown?

    pauseMenuButton{topRight + bufferH - bufferW - TDT4102::Point{btnW, 0}, btnW, btnH, "resume"},
    subMenuButton{subMenuTopLeft + 4 * bufferH, btnW, btnH, "load"},
    //========================= Textfield

    fileInput{subMenuTopLeft + 3 * bufferH, fieldW, fieldH, "filename"},

    //========================= Slider

    radiusSlider{bottomLeft - bufferH + bufferW, fieldW, fieldH},
    edgeSlider{bottomLeft - bufferH + 5* bufferW, fieldW, fieldH}
    {
    importMenuButton.setCallback(std::bind(importMenuCallback, this));
    exportMenuButton.setCallback(std::bind(exportMenuCallback, this));
    infoMenuButton.setCallback(std::bind(infoMenuCallback, this));
    colorMenuButton.setCallback(std::bind(colorMenuCallback, this));
    
    pauseMenuButton.setCallback(std::bind(pauseMenuCallback, this));
    subMenuButton.setCallback(std::bind(subMenuCallback, this));

    add(importMenuButton);
    add(exportMenuButton);
    add(pauseMenuButton);
    add(subMenuButton);
    add(infoMenuButton);
    add(colorMenuButton);
    add(fileInput);
    add(radiusSlider);
    add(edgeSlider);

    updateGUI();
}

void GraphWindow::run() {
    while(!should_close()){
        updateGUI();
        switch (currState) {
            case infoMenu:
            case colorMenu:
            case exportMenu:
            case importMenu:
            case mainMenu:
                if (currState != mainMenu) {
                    drawSubMenu();
                }
                drawMenu();
                next_frame();
                break;

            case selectingNextNode:
            case mainScreen:
                updateMain();
                drawMain();
                next_frame();
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
    if (framesSinceLastHoveredRightClick < 100) {
        framesSinceLastHoveredRightClick++;
    }

    switch (currState) {
    case mainScreen:
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
        if (is_right_mouse_button_down() && framesSinceLastHoveredRightClick < 10 && !rightMouseButtonClick) {
            for (auto& it : nodes) {
                it -> setSelect(false);
            }
            hoveredNode -> setSelect(true);
            currState = selectingNextNode;

            framesSinceLastHoveredRightClick = 0;
            rightMouseButtonClick = true;
        }
        else if (is_right_mouse_button_down() && !rightMouseButtonClick) {
            hoveredNode -> updateSelect();
            rightMouseButtonClick = true;

            framesSinceLastHoveredRightClick = 0;
        }
        else if (!is_right_mouse_button_down()) {
            rightMouseButtonClick = false;
        }

        if (is_key_down(KeyboardKey::A) && !aClick) {
            for (auto& it : nodes) {
                it -> setSelect(false);
            }
            hoveredNode -> setSelect(true);

            tempNode = std::make_shared<Node>(get_mouse_coordinates(), "");
            tempWeight = getInputWeight();

            currState = selectingNextNode;
            aClick = true;
        }
        else if (!is_key_down(KeyboardKey::A)) {
            aClick = false;
        }
    }
    //================================================== Delete

    if (is_key_down(KeyboardKey::BACKSPACE)) {
        removeSelectedEdges();
    }
    if (is_key_down(KeyboardKey::DELETE)){
        removeSelectedNodes();
    }
    //================================================== Reset
    if (is_key_down(KeyboardKey::R)) {
        resetLabels();
    }

    //================================================== Add

    if (is_key_down(KeyboardKey::PLUS) && !plusClick) {
        addNode(get_mouse_coordinates(), nextLabel);
        updateNextLabel();
        plusClick = true;
    }
    else if (!is_key_down(KeyboardKey::PLUS)) {
        plusClick = false;
    }

    if (is_key_down(KeyboardKey::E)) {
        addSelectedEdges(getInputWeight());
    }
    break;
    case selectingNextNode:
        if (tempNode) {
                tempNode -> setLocation(get_mouse_coordinates());
            }
        if (std::shared_ptr<Node> hoveredNode{getNode(currMouseLocation)}) {
            while (is_left_mouse_button_down()) {
                hoveredNode -> setLocation(hoveredNode -> getLocation() + deltaMouseMovement);
                drawMain();
                next_frame();
                lastMouseLocation = currMouseLocation;
                currMouseLocation = get_mouse_coordinates();
                deltaMouseMovement = currMouseLocation - lastMouseLocation;
                framesSinceLastHoveredLeftClick = 0;
            }
            
            auto previousNode = getSelectedNodes()[0];
            if (hoveredNode == previousNode) break;
            if (is_right_mouse_button_down() && !rightMouseButtonClick) {
                if (tempNode) {
                    addDirectionalEdge(previousNode, hoveredNode, tempWeight);
                    tempNode = nullptr;
                }
                else {
                    pathLength = getShortestPath(previousNode, hoveredNode);
                }
                previousNode -> setSelect(false);
                framesSinceLastHoveredRightClick = 0;
                rightMouseButtonClick = true;
                currState = mainScreen;
            }
            else if (is_right_mouse_button_down()) {
                framesSinceLastHoveredRightClick = 0;
                rightMouseButtonClick = true;
            }
            else  {
                rightMouseButtonClick = false;
            }
        }
    }

    //================================================== DRAG

    while (is_left_mouse_button_down()) {
        for (auto & it : nodes) {
            it -> setLocation(it -> getLocation() + 0.8 * deltaMouseMovement);
        }
        relativeWindowCoordinates = relativeWindowCoordinates - 0.8 * deltaMouseMovement;
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
    drawMouseCoordinates();
    drawPathLength();
}

void GraphWindow::drawMenu() {
    draw_rectangle(mainMenuTopLeft, 2 * menuW, menuH, menuColor);
    draw_text(mainMenuTopLeft + TDT4102::Point{200, bufH}, "Menu", TDT4102::Color::black, 48U, TDT4102::Font::arial);
    lastMouseLocation = get_mouse_coordinates();
}

void GraphWindow::drawSubMenu() {
    draw_rectangle(subMenuTopLeft, menuW, menuH, subMenuColor);
}


void GraphWindow::drawNode(const std::shared_ptr<Node>& node){ 
    draw_circle(node -> getLocation(), radius, node -> isSelected() ? selectedNodeColor : unselectedNodeColor,borderColor);

    draw_text(node -> getLocation() + TDT4102::Point{-15, -15}, node -> getLabel());
}
void GraphWindow::drawEdge(const TDT4102::Point& startPoint, const TDT4102::Point& endPoint, const bool selected, const int& weight){
    TDT4102::Point tangent = getTangent(endPoint - startPoint);
    TDT4102::Point normal = getNormal(endPoint - startPoint);
    double distance = getDistance(startPoint, endPoint);

    draw_quad(
        startPoint + 0.1 * radius * tangent + (edgeW * normal),
        endPoint - 0.1 * radius * tangent + (edgeW * normal),
        endPoint - 0.1 * radius * tangent -(edgeW * normal),
        startPoint + 0.1 * radius * tangent - (edgeW * normal),
        selected ? selectedEdgeColor : unselectedEdgeColor
    );
    if (distance > 5 * radius) {
        draw_triangle(
            endPoint -  0.06 * radius * tangent,
            endPoint - 0.5 * edgeW * radius * tangent +  3 * edgeW * normal,
            endPoint - 0.5 * edgeW * radius * tangent - 3 * edgeW * normal,
            selected ? selectedEdgeColor : unselectedEdgeColor
        );
    }
    else {
        draw_triangle(
            endPoint -  0.06 * radius * tangent,
            endPoint - 0.5 * edgeW * radius * (distance/(5 * radius)) * tangent +  3 * edgeW * (distance/(5 * radius)) * normal,
            endPoint - 0.5 * edgeW * radius * (distance/(5 * radius)) * tangent - 3 * edgeW * (distance/(5 * radius)) * normal,
            selected ? selectedEdgeColor : unselectedEdgeColor
        );
    }
    if (weight != 1) {
        draw_text(
            startPoint + 0.5 * (endPoint - startPoint),
            std::to_string(weight)
        );
    }
    
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
                    drawEdge(from -> getLocation(), to -> getLocation(), it -> getSelect(), it -> getWeight());
                }
            }
        }
    }
    if (tempNode) {
        drawEdge(getSelectedNodes()[0] -> getLocation(), tempNode -> getLocation(), false, tempWeight);
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

void GraphWindow::pauseMenuCallback() {
    switch (currState) {
        case selectingNextNode:
        case mainScreen:
            currState = mainMenu;
            pauseMenuButton.setLabel("resume");
            break;

        default:
            currState = mainScreen;
            pauseMenuButton.setLabel("pause");
            break;
    }
}

void GraphWindow::exportMenuCallback(){
    switch (currState) {
        case exportMenu:
            currState = mainMenu;
            break;
        default:
            currState = exportMenu;
    }
}
void GraphWindow::importMenuCallback(){
    switch (currState) {
        case importMenu:
            currState = mainMenu;
            break;
        default:
            currState = importMenu;
    }
}
void GraphWindow::colorMenuCallback(){
    switch (currState) {
        case colorMenu:
            currState = mainMenu;
            break;
        default:
            currState = colorMenu;
    }
}
void GraphWindow::infoMenuCallback(){
    switch (currState) {
        case infoMenu:
            currState = mainMenu;
            break;
        deafult:
            currState = infoMenu;
    }
}

void GraphWindow::subMenuCallback() {
    switch (currState) {
        case importMenu:
            loadButtonCallback();
            break;
        case exportMenu:
            saveButtonCallback();
            break;
        default:
            std::cerr << "SubMenu while in wrong state\n";
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

void GraphWindow::updateGUI() {
    switch (currState) {
        case importMenu:
            subMenuButton.setLabel("load");
        case exportMenu:
            if (currState == exportMenu) {
                subMenuButton.setLabel("save");
            }
            subMenuButton.setVisible(true);
            fileInput.setVisible(true);
        case infoMenu:
        case colorMenu:
        case mainMenu:
            if (!(currState == importMenu || currState == exportMenu)) {
                subMenuButton.setVisible(false);
                fileInput.setVisible(false);
                fileInput.setText("filename");
            }

            radiusSlider.setVisible(false);
            edgeSlider.setVisible(false);

            importMenuButton.setVisible(true);
            exportMenuButton.setVisible(true);
            infoMenuButton.setVisible(true);
            colorMenuButton.setVisible(true);
            break;

        default:
            radiusSlider.setVisible(true);
            edgeSlider.setVisible(true);

            subMenuButton.setVisible(false);
            fileInput.setVisible(false);
            importMenuButton.setVisible(false);
            exportMenuButton.setVisible(false);
            infoMenuButton.setVisible(false);
            colorMenuButton.setVisible(false);
    }
}

void GraphWindow::drawMouseCoordinates() {
    TDT4102::Point coords = get_mouse_coordinates();
    std::string s = "x: " + std::to_string(relativeWindowCoordinates.x + coords.x) + " y: " + std::to_string(relativeWindowCoordinates.y + coords.y);
    draw_text(
        topLeft + bufferW,
        s
    );
}

void GraphWindow::drawPathLength() {
    std::string s = "path length: " + std::to_string(pathLength);
    draw_text(
        topLeft + buffer,
        pathLength < 9999 ? s : "No valid path"
    );
}