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
}

GraphWindow::GraphWindow(std::filesystem::path fileName):
    Graph(fileName),
    AnimationWindow(50, 50, Graph::width, Graph::height, "Grafvisualisering"),

    loadButton{TDT4102::Point{100,100}, btnW, btnH, "load"},
    saveButton{TDT4102::Point{100, 200}, btnW, btnH, "save"},

    fileInput{TDT4102::Point{100, 300}, fieldW, fieldH, "filename"}
    {}

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