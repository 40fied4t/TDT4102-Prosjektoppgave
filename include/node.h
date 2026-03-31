#pragma once

#include <memory>
#include "AnimationWindow.h"

/// @brief Klasse som representerer nodene i et grafnettverk.
/// @see Graph
/// @see Edge
class Node
{
private:
    std::string label;                  ///< Nodens navn.
    bool selected = false;              ///< Bool som viser om noden er valgt.
    TDT4102::Point location;         ///< Nodens lokasjon som Point, til bruk i visualsering.
    int count = 0;
public:
    /// @brief Klassens konsturktør.
    /// @param location Lokasjonen noden skal konstruerer på.
    /// @param label Navnet noden skal konstrueres med
    Node(TDT4102::Point location, std::string label):
    label{label},
    location{location}
    {}

    /// @brief Gir nodens nåværende lokasjon.
    /// @return location.
    TDT4102::Point getLocation() const {return this -> location;}

    /// @brief Gir nodens nåværende navn.
    /// @return label.
    std::string getLabel() const {return this -> label;}

    /// @brief Setter nytt navn for noden.
    /// @param newLabel det nyet navnet til noden.
    void setLabel(std::string newLabel) {this -> label = newLabel;}

    /// @brief Viser om noden er valgt.
    /// @return selected.
    bool isSelected() const {return this -> selected;}


    /// @brief Tilegner selected = !selected.
    void updateSelect() {this -> selected = !this -> selected;}

    void setSelect(const bool truthVal) {this -> selected = truthVal;}

    void setLocation(const TDT4102::Point location) {this -> location = location;}

    int getCount() const {return count;}
    void setCount(const int& c) {count = c;}
};







