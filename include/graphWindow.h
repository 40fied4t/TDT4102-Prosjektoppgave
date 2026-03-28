#pragma once

#include "include/graph.h"
#include "widgets/Button.h"
#include "widgets/TextInput.h"
class GraphWindow : public TDT4102::AnimationWindow, public Graph
{
private:
    //========================= Constants
    static constexpr int bufW = 20;
    static constexpr int bufH = 20;
    static constexpr int btnH = 20;
    static constexpr int btnW = 20;

    static constexpr int fieldW = 100;
    static constexpr int fieldH = 20;

    static constexpr int radius = 50;

    //========================= States

    enum WindowState {
        menu,
        main,
        closed
    };
    WindowState currState = menu;

    //========================= Buttons

    TDT4102::Button loadButton;
    TDT4102::Button saveButton;

    TDT4102::TextInput fileInput;

public:
    //========================= Init
    GraphWindow();
    GraphWindow(std::filesystem::path fileName);
    ~GraphWindow();
    
    void run();
    //========================= Draw
    void drawMenu();
    void drawMain();

    void drawNode(const std::shared_ptr<Node>& node);
    void drawEdge(const std::unique_ptr<Edge>& edge);

    //========================= Buttons
    void loadButtonCallback();
    void saveButtonCallback();
    
};