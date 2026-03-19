#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include "AnimationWindow.h"
//Egendefinerte typer
typedef std::vector<std::shared_ptr<Node>> NodeVec;
typedef std::unordered_map<std::shared_ptr<Node>, NodeVec> GraphMap;
typedef std::vector<std::vector<std::shared_ptr<Node>>> EdgeVec;

class Graph
{
private:
    int numNodes;
    int nextLabel;
    GraphMap graphMap;
    EdgeVec edgeVec;
    NodeVec nodes;
    NodeVec selectedNodes;

public:
    void updateSelectedNodes();
    void addEdges();
    void removeEdges();
    void addNode(const TDT4102::Point location, const int& label);
    void removeNode();
    void removeSelectedNodes();
    int getSize();
    int getEdgeNum();


    Graph();
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
    TDT4102::Point getLocation() const;
    int getLabel() const;
    bool isSelected() const;
    void updateSelect();
};