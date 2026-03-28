#pragma once

#include "include/graph.h"

class GraphWindow : public TDT4102::AnimationWindow, public Graph
{
private:
    enum class WindowState {
        menu,
        main
    };

    static constexpr int bufW = 20;
    static constexpr int bufH = 20;
public:
    //========================= Init
    GraphWindow();
    ~GraphWindow();

    void run();

    //========================= Draw
    void drawMenu();
    void drawMain();

    void drawNode(const std::shared_ptr<Node>& node);
    void drawEdge(const std::unique_ptr<Edge>& edge);

    //=========================

};