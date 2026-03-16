#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "AnimationWindow.h"
#include "Widget.h"

//Egendefinerte typer
typedef std::unordered_map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> GraphMap;
typedef std::vector<std::shared_ptr<Node>> NodeVec;
typedef std::vector<std::shared_ptr<Edge>> EdgeVec;

class Graph
{
private:
    int numNodes = 1;
    int nextLabel = 1;
    GraphMap graphMap;
public:
    friend std::ostream& operator<<(std::ostream& os, const GraphMap& map);
    friend std::istream& operator>>(std::istream& is, const GraphMap& map);
};

class Node
{
private:
    const int label;
    static constexpr int radius = 25;
    bool selected = false;
    TDT4102::Point  location;

    EdgeVec edges;
public:
    Node(TDT4102::Point location);
    Node(TDT4102::Point location, std::vector<std::shared_ptr<Node>> others);
    ~Node();

    void addEdge(std::shared_ptr<Node> other);
    void removeEdge(std::shared_ptr<Node> other);

    std::shared_ptr<Node> getOther(std::shared_ptr<Edge> edge);
    std::shared_ptr<Edge> getEdge(std::shared_ptr<Node> other);

    friend class Edge;
};

class Edge 
{

};