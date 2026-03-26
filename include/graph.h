#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include "AnimationWindow.h"
#include "exception.h"

class Node
{
private:
    std::string label;
    static constexpr int radius = 25;
    bool selected = false;
    TDT4102::Point  location;
public:
    Node(TDT4102::Point location, std::string label);
    TDT4102::Point getLocation() const;
    std::string getLabel() const;
    bool isSelected() const;
    void updateSelect();
};

class Edge
{
private:
    const int weight;
protected:
    std::vector<std::shared_ptr<Node>> nodeVec;
public:
    
    virtual std::vector<std::shared_ptr<Node>> getTo() const;
    virtual std::vector<std::shared_ptr<Node>> getFrom() const;
    virtual char getDelim(){return '-';}
    int getWeight() const;

    Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, const int weight);
    Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
};

class DirectionalEdge : public Edge
{

public:
    virtual std::vector<std::shared_ptr<Node>> getTo() const override;
    virtual std::vector<std::shared_ptr<Node>> getFrom() const override;
    virtual char getDelim() override {return '>';}
    DirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, const int weight);
    DirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
};
class Graph
{
private:
    static constexpr int width = 1000;
    static constexpr int height = 500;

    std::string nextLabel = "NULL";
    std::vector<std::string> labelVec;

    std::unordered_map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> graphMap;
    std::vector<std::unique_ptr<Edge>> edgeVec;
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Node>> selectedNodes;

public:
    void updateSelectedNodes();
    void updateNextLabel();

    void addEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, const int weight);
    void addDirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to , const int weight);
    
    void addSelectedEdges(const int weight);

    void removeEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
    void removeAllEdgesToNode(std::shared_ptr<Node> node);
    void removeSelectedEdges();
    void addNode(const TDT4102::Point location, std::string label);
    void removeNode();
    void removeSelectedNodes();
    int getSize() const;
    int getEdgeNum() const;

    std::shared_ptr<Node> getNode(const std::string& label) const;
    std::vector<TDT4102::Point> generatePositions(const int& n);
    void empty();
    template <typename T>
    bool leadsTo(std::unordered_map<T, std::vector<T>> map, T from, T to) {
        for (auto &it : map[from]) {
            if (it == to) {return true;}
        }
        return false;
    }

    Graph();
    Graph(std::filesystem::path fileName);
    ~Graph();

    void loadFromAdj(std::filesystem::path fileName);
    void loadFromEdg(std::filesystem::path fileName);

    void saveToAdj(std::filesystem::path fileName);
    void saveToEdg(std::filesystem::path fileName);
};