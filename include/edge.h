#pragma once

#include "include/node.h"


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

    bool selected = false;
public:
    /// @brief Gir nodeVec
    /// @return nodeVec
    const std::vector<std::shared_ptr<Node>> getNodeVec() const {return nodeVec;}
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

    void setSelect(const bool truthVal) {selected = truthVal;}
    bool getSelect() const {return selected;}
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

    /// @brief Klassens konstruktør. 
    /// @param from Legges til som den første noden i nodeVec
    /// @param to Legges til som den andre noden i nodeVec
    /// @param weight Legges til som weight, settes til 1 dersom annet ikke oppgis
    DirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, const int weight);
    DirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
};
