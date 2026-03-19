#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "AnimationWindow.h"
//Egendefinerte typer
typedef std::vector<std::shared_ptr<Node>> NodeVec;
typedef std::unordered_map<std::shared_ptr<Node>, NodeVec> GraphMap;


class Graph
{
private:
    int numNodes;
    int nextLabel;
    GraphMap graphMap;
    NodeVec nodes;
    NodeVec selectedNodes;

protected:
    void updateSelectedNodes();

    void addEdges();
    void removeEdges();

    void addNode(const TDT4102::Point location, const int& label);
    void removeNode();
    void removeSelectedNodes();


public:
    Graph();
    ~Graph();

    friend std::ostream& operator<<(std::ostream& os, const GraphMap& map);
    friend std::istream& operator>>(std::istream& is, GraphMap& map);
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
};