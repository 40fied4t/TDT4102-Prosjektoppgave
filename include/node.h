#pragma once

#include <memory>
#include <vector>
#include "AnimationWindow.h"
#include "Widget.h"


class Node : public TDT4102::Widget
{
private:
    std::string label;
    static constexpr unsigned int nodeRadius = 25;
    static constexpr unsigned int widgetSize = 20;

protected:
    void update(nk_context* context) override;
public:
    explicit Node(TDT4102::Point location);
};