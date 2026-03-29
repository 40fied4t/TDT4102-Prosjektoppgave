#pragma once

#include <cmath>

#include "include/graph.h"
#include "widgets/Button.h"
#include "widgets/TextInput.h"
class GraphWindow : public TDT4102::AnimationWindow, public Graph
{
private:
    //========================= Variables
    TDT4102::Point lastMouseLocation{0,0};

    bool rightMouseButtonClick = false;
    int framesSinceLastLeftClick = 0;
    //========================= Constants
    static constexpr int bufW = 30;
    static constexpr int bufH = 30;
    
    static constexpr int btnW = 100;
    static constexpr int btnH = 50;

    static constexpr int fieldW = 200;
    static constexpr int fieldH = 50;

    static constexpr int radius = 20;

    //========================= States

    enum WindowState {
        menu,
        main,
        closed
    };
    WindowState currState = main;

    //========================= Buttons

    TDT4102::Button loadButton;
    TDT4102::Button saveButton;
    TDT4102::Button changeMenuButton;

    TDT4102::TextInput fileInput;

    //========================= Colors

    TDT4102::Color unselectedNodeColor = TDT4102::Color::purple;
    TDT4102::Color selectedNodeColor = TDT4102::Color::blue_violet;

    TDT4102::Color borderColor = TDT4102::Color::transparent;

    TDT4102::Color unselectedEdgeColor = TDT4102::Color::black;
    TDT4102::Color selectedEdgeColor = TDT4102::Color::blue;
public:
    //========================= Init

    GraphWindow();
    GraphWindow(std::filesystem::path fileName);
    void run();
    //========================= Get

    std::shared_ptr<Node> getNode(const TDT4102::Point& location) const;
    
    //========================= Update

    void updateMain();
    
    //========================= Draw
    void drawMenu();
    void drawMain();

    void drawNode(const std::shared_ptr<Node>& node);
    void drawEdge(const TDT4102::Point& startPoint, const TDT4102::Point& endPoint, const bool selected);

    void drawAllNodes();
    void drawAllEdges();

    //========================= Buttons
    void loadButtonCallback();
    void saveButtonCallback();
    void changeMenuCallback();
    
};

