#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <iostream>

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

    /// @brief Setter nytt navn for noden.
    /// @param newLabel det nyet navnet til noden.
    void setLabel(const std::string& newLabel) {label = newLabel;}

    /// @brief Viser om noden er valgt.
    /// @return selected.
    bool isSelected() const;

    /// @brief Tilegner selected = !selected.
    void updateSelect();
public:
    friend class Graph;
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
public:
    friend class Graph;
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

/// @brief Overordnet datastruktur for grafen.
/// @attention Klassen er ansvarlig for minnet til både Node og Edge via smart_ptr.
/// @see Node
/// @see Edge
/// @see GraphWindow
class Graph
{
private:
    static constexpr int width = 1000;  ///< Vinduets bredde.
    static constexpr int height = 500;  ///< Vinduets høyde.

    std::string nextLabel = "1";     ///< Navnet som blir gitt neste genererte node.
    std::vector<std::string> labelVec;  ///< Referansevektor for opptatte navn.

    /// @brief Oppslagsverk for hvor en node fører
    /// Dersom en node from leder til en annen node to, inneholder graphMap[from] to.
    std::unordered_map<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> graphMap;

    /// @brief Vektor med unique_ptr til alle kantene grafen inneholder.
    std::vector<std::unique_ptr<Edge>> edgeVec;

    /// @brief Vektor med shared_ptr til alle kanter grafen inneholder.
    std::vector<std::shared_ptr<Node>> nodes;

    /// @brief Vektor med shared_ptr til valgte noder.
    /// @note Oppdateres via updateSelectedNodes(), og funksjoner som kaller denne.
    std::vector<std::shared_ptr<Node>> selectedNodes;


public:
    /// @brief Oppdaterer selectedNodes
    /// Kaller selectedNodes.clear(), og legger så til noder fra nodes der Node::isSelected() == true.
    void updateSelectedNodes();
    /// @brief Finner neste heltallsnavn som ikke er tatt i bruk og oppdaterer nextLabel.
    void updateNextLabel();

    void resetLabels();

    /// @brief Legger til urettet kant mellom to noder.
    /// @param from Den første noden.
    /// @param to Den andre noden.
    /// @param weight Vekting av kanten.
    void addEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to, const int weight);

    /// @brief Legger til en rettet kant mellom to noder.
    /// @param from Noden kanten har utløp fra.
    /// @param to Noden kanten leder til.
    /// @param weight Vekting av kanten.
    void addDirectionalEdge(std::shared_ptr<Node> from, std::shared_ptr<Node> to , const int weight);
    
    /// @brief Legger til urettede kanter mellom alle valgte kanter, som ikke allerede har kanter mellom seg.
    /// @param weight Vekting av kantene.
    void addSelectedEdges(const int weight);

    /// @brief Fjerner kant mellom to noder dersom det er en kant mellom dem.
    /// @param first Den første noden.
    /// @param second Den andre noden.
    void removeEdge(std::shared_ptr<Node> first, std::shared_ptr<Node> second);

    /// @brief Fjerner alle kantene som 
    /// @param node 
    void removeAllEdgesOfNode(std::shared_ptr<Node> node);

    /// @brief Fjerner kanter mellom alle valgte kanter, dersom det er kanter mellom dem.
    void removeSelectedEdges();

    /// @brief Legger til en ny node i grafen.
    /// @param location Lokasjonen til den nye noden.
    /// @param label Navnet til den nye noden.
    void addNode(const TDT4102::Point location, std::string label);

    /// @brief Fjerner en node fra grafen.
    /// Kaller først removeAllEdgesOfNode().
    /// @param node Node som skal fjernes.
    void removeNode(std::shared_ptr<Node> node);

    /// @brief Fjerner alle valgte noder fra grafen ved bruk av removeNode.
    void removeSelectedNodes();

    /// @brief Gir antall noder i grafen.
    /// @return nodes.size()
    int getSize() const;

    /// @brief Gir antall kanter i grafen.
    /// @return edgeVec.size()
    int getEdgeNum() const;

    /// @brief Gir node basert på navn
    /// @param label navnet som  
    /// @return node
    /// @warning Returnerer nullptr dersom det ikke finnes node ved dette navnet.
    std::shared_ptr<Node> getNode(const std::string& label) const;

    /// @brief Genererer n ikke-overlappende posisjoner innenfor vinduet.
    /// @param n Antall poisjoner
    /// @return Vektor med posisjoner
    std::vector<TDT4102::Point> generatePositions(const int& n);

    /// @brief Tømmer alle beholdere i Graph unntatt labelVec.
    void empty();

    /// @brief Hjelpefunksjon for å finne om to ligger i vektoren map[from].
    /// @tparam T 
    /// @param map Mappet som skal letes i
    /// @param from Nøkkelverdi
    /// @param to Verdien som letes etter
    /// @return true if to in map[form] else false.
    template <typename T>
    bool leadsTo(std::unordered_map<T, std::vector<T>> map, T from, T to) {
        for (auto &it : map[from]) {
            if (it == to) {return true;}
        }
        return false;
    }
    
    /// @brief Klassens konstruktør, oppretter én node i midten av vinduet.
    Graph();

    /// @brief Konstruktør for klassen fra en fil, med format ".adj"/".edg".
    /// @param fileName Filsti for filen klassen skal lastes inn fra.
    /// Bruker henholdsvis loadFromAdj eller loadFromEdg avhengig av hvilken filtype konstruktøren kalles med.
    Graph(std::filesystem::path fileName);
    /// @brief Klassens dekstruktør.
    ~Graph();

    /// @brief Laster inn grafstruktur som adjacency list fra .adj fil. Vekter alle kanter likt.
    /// @param fileName Filsti, må være .adj fil.
    /// @note Endrer ikke på grafen dersom det er feil ved filformat, eller filen ikke er en .adj fil.
    /// @attention Erstatter den nåverende grafstrukturen, ved bruk av empty().
    /// Hver linje i .adj fil skal være formatert som en node etterfulgt av nodene man kan nå via noden eksempelvis: node : node1 node2 node3.
    /// Alle nodene ha sin egen linje i listen, og være etterfulgt av kolon. Videre er navnet på nodene formatert som streng, og kan ikke inneholde mellomrom.
    void loadFromAdj(std::filesystem::path fileName);

    /// @brief Laster inn graftstruktur som (litt modifisert) edge list fra .edg fil, med vekting av kanter.
    /// @param fileName Filsti, må være .edg fil.
    /// Hver line i i .edg fil skal være formatert som to noder, med type kant imellom dem, etterfulgt av vekting.
    /// Eksempelvis:
    ///     node1 - node2 1
    ///     node1 > node3 2 
    ///     node2 < node3 3
    /// Dette gir en urettet kant mellom node1 og node2 med vekting 1, en rettet kant fra node1 til node3 med vekting 2, og en rettet kant fra node3 til node2 med vekting 3.
    /// @note Merk at det kun kan eksistere én kant mellom to gitte noder slik grafen er implementert. Dermed vil linjer som legger til kanter mellom to noder som allerede har en kant ikke gjøre noe.
    void loadFromEdg(std::filesystem::path fileName);

    /// @brief  Laster opp grafstruktur som adjacency list til .adj fil. Vekter alle kanter likt.
    /// @param fileName Filsti, må være .adj fil.
    void saveToAdj(std::filesystem::path fileName);

    /// @brief Laster opp grafstuktur som edge list til .edg fil, med vekting.
    /// @param fileName Filsti, må være .edg fil.
    void saveToEdg(std::filesystem::path fileName);
};