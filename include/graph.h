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

/// @brief Klasse som representerer nodene i et grafnettverk.
/// @see Graph
/// @see Edge
class Node
{
private:
    std::string label;                  ///< Nodens navn.
    bool selected = false;              ///< Bool som viser om noden er valgt.
    TDT4102::Point  location;           ///< Nodens lokasjon som Point, til bruk i visualsering.
public:
    /// @brief Klassens konsturktør.
    /// @param location Lokasjonen noden skal konstruerer på.
    /// @param label Navnet noden skal konstrueres med
    Node(TDT4102::Point location, std::string label);

    /// @brief Gir nodens nåværende lokasjon.
    /// @return location.
    TDT4102::Point getLocation() const;

    /// @brief Gir nodens nåværende navn.
    /// @return label.
    std::string getLabel() const;

    /// @brief Viser om noden er valgt.
    /// @return selected.
    bool isSelected() const;

    /// @brief Tilegner selected = !selected.
    void updateSelect();
};

/// @brief Klasse som representerer kantene i nettverket.
/// @see DirectionalEdge
/// @see Graph
/// @see Node
class Edge
{
private:
    const int weight;                                           ///< Vektingen til kanten.
protected:
    /// @brief Vektor som inneholder nodene kanten peker til.
    /// @note Implementert som vektor med to elementer, men støtter større lengder, men dette støttes ikke av implementert filformat.
    /// @warning Medlemsklasser slik som DirectionalEdge kan avhenge av at vektoren har en gitt struktur f.eks. {from, to}.
    std::vector<std::shared_ptr<Node>> nodeVec;
public:
    /// @brief Gir hvilke noder som kan nåes av kanten.
    /// @return nodeVec
    virtual std::vector<std::shared_ptr<Node>> getTo() const;
    /// @brief Gir hvilke noder som kanten har utløp fra.
    /// @return nodeVec
    virtual std::vector<std::shared_ptr<Node>> getFrom() const;
    /// @brief Returner hvilket delimsymbol som skal brukes for Graph::saveToEdg()
    /// @return '-'
    virtual char getDelim(){return '-';}

    /// @brief Gir vektingen til kanten.
    /// @return weight
    int getWeight() const;

    /// @brief Klassens kontruktør. 
    /// @param from Legges til som den første noden i nodeVec
    /// @param to Legges til som den andre noden i nodeVec
    /// @param weight Legges til som weight, settes til 1 dersom annet ikke oppgis
    Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, const int weight);
    Edge(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
};

/// @brief Medlem av Edge som kun går én vei.
class DirectionalEdge : public Edge
{

public:
    /// @brief Gir noden som kan nåes av kanten.
    /// @return nodeVec[1]
    virtual std::vector<std::shared_ptr<Node>> getTo() const override;
    /// @brief Gir noden som kanten har utløp fra.
    /// @return nodeVec[0]
    virtual std::vector<std::shared_ptr<Node>> getFrom() const override;
    /// @brief Returner hvilket delimsymbol som skal brukes for Graph::saveToEdg()
    /// @return '>'
    virtual char getDelim() override {return '>';}

    /// @brief Klassens kontruktør. 
    /// @param from Legges til som den første noden i nodeVec
    /// @param to Legges til som den andre noden i nodeVec
    /// @param weight Legges til som weight, settes til 1 dersom annet ikke oppgis
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