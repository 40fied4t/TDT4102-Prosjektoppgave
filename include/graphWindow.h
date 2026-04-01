#pragma once

#include <cmath>

#include "include/graph.h"
#include "pointOperations.h"

#include "widgets/Button.h"
#include "widgets/TextInput.h"
#include "widgets/Slider.h"

class GraphWindow : public TDT4102::AnimationWindow, public Graph
{
private:
    //========================= Variables
    TDT4102::Point lastMouseLocation{0,0};
    TDT4102::Point relativeWindowCoordinates{0,0};

    bool rightMouseButtonClick = false;
    bool aClick = false;
    bool plusClick = false;

    int framesSinceLastHoveredLeftClick = 0;
    int framesSinceLastHoveredRightClick = 0;

    int pahtLength = 9999;

    TDT4102::Point bottomRight{1024, 768};
    TDT4102::Point bottomLeft{0, 768};
    TDT4102::Point topRight{1024, 0};
    static constexpr TDT4102::Point topLeft{0,0};

    int radius = 20;
    double edgeW = 0.5;

    /**********MENU***********/
    TDT4102::Point mainMenuTopLeft{256, 192};
    TDT4102::Point subMenuTopLeft{512, 192};

    int menuH = 384;
    int menuW = 256;

    //========================= Constants
    static constexpr int bufW = 50;
    static constexpr int bufH = 50;
    static constexpr TDT4102::Point buffer{bufW, bufH};
    static constexpr TDT4102::Point bufferH{0, bufH};
    static constexpr TDT4102::Point bufferW{bufW, 0};
    
    static constexpr int btnW = 100;
    static constexpr int btnH = 50;

    static constexpr int fieldW = 200;
    static constexpr int fieldH = 50;

    

    //========================= States

    enum WindowState {
        mainMenu,
            exportMenu,
            importMenu,
            infoMenu,
            colorMenu,

        mainScreen,
            selectingNextNode
    };
    WindowState currState = mainMenu;

    //========================= Buttons

    TDT4102::Button importMenuButton;
    TDT4102::Button exportMenuButton;
    TDT4102::Button pauseMenuButton;
    TDT4102::Button subMenuButton;
    TDT4102::Button infoMenuButton;
    TDT4102::Button colorMenuButton; // Maybe dropdown?

    //========================= Textfield

    TDT4102::TextInput fileInput;

    //========================= Slider

    TDT4102::Slider radiusSlider;
    TDT4102::Slider edgeSlider;
    
    //========================= Colors

    TDT4102::Color unselectedNodeColor = TDT4102::Color::purple;
    TDT4102::Color selectedNodeColor = TDT4102::Color::blue_violet;

    TDT4102::Color borderColor = TDT4102::Color::transparent;

    TDT4102::Color unselectedEdgeColor = TDT4102::Color::black;
    TDT4102::Color selectedEdgeColor = TDT4102::Color::blue;

    TDT4102::Color menuColor = TDT4102::Color::gray;
    TDT4102::Color subMenuColor = TDT4102::Color::gray;
public:
    //========================= Init

    GraphWindow();
    GraphWindow(std::filesystem::path fileName);
    void run();
    //========================= Get

    std::shared_ptr<Node> getNode(const TDT4102::Point& location) const;
    //========================= Update

    void updateMain();
    void updateGUI();
    
    //========================= Draw
    void drawMenu();
    void drawMain();
    void drawSubMenu();

    void drawNode(const std::shared_ptr<Node>& node);
    void drawEdge(const TDT4102::Point& startPoint, const TDT4102::Point& endPoint, const bool selected, const int& weight);

    void drawAllNodes();
    void drawAllEdges();

    void drawMouseCoordinates();
    void drawPathLength();

    //========================= Callbackfunctions;
    void loadButtonCallback();
    void saveButtonCallback();
    void pauseMenuCallback();
    void exportMenuCallback();
    void importMenuCallback();
    void colorMenuCallback();
    void infoMenuCallback();
    void subMenuCallback();
    //========================= Input

    int getInputWeight() const;

};

