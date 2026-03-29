#include "include/pointOperations.h"

TDT4102::Point operator-(const TDT4102::Point& first, const TDT4102::Point& second) {
    return TDT4102::Point{first.x - second.x, first.y - second.y};
}

TDT4102::Point operator+(const TDT4102::Point& first, const TDT4102::Point& second){
    return TDT4102::Point{first.x + second.x, first.y + second.y};
}

TDT4102::Point operator*(const float& f, const TDT4102::Point& pointVector) {
    return TDT4102::Point{static_cast<int>(f * pointVector.x), static_cast<int>(f * pointVector.y)};
}