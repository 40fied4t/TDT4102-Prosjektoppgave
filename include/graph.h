#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>

#include "AnimationWindow.h"
//Egendefinerte typer
// typedef std::vector<std::shared_ptr<Node>> NodeVec;
// typedef std::unordered_map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> GraphMap;
// typedef std::vector<std::vector<std::shared_ptr<Node>>> EdgeVec;

class Graph
{
private:
    static constexpr int width = 1000;
    static constexpr int height = 500;

    int numNodes = 0;
    int nextLabel = 1;
    std::unordered_map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> graphMap;
    std::vector<std::vector<std::shared_ptr<Node>>> edgeVec;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Node>> selectedNodes;

public:
    void updateSelectedNodes();
    void updateNextLabel();
    void addEdges();
    void removeEdges();
    void addNode(const TDT4102::Point location, const int& label);
    void removeNode();
    void removeSelectedNodes();
    int getSize();
    int getEdgeNum();


    Graph() = default;
    Graph(std::filesystem::path fileName);
    ~Graph();

    void loadFromAdj(std::filesystem::path fileName);
    void loadFromEdg(std::filesystem::path fileName);

    void saveToAdj(std::filesystem::path fileName);
    void saveToEdg(std::filesystem::path fileName);
};

class Node
{
private:
    const int label;
    static constexpr int radius = 25;
    bool selected = false;
    TDT4102::Point  location;
public:
    Node(TDT4102::Point location, const int& label);
    ~Node();
    TDT4102::Point getLocation() const;
    int getLabel() const;
    bool isSelected() const;
    void updateSelect();
};