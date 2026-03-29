#include "include/pointOperations.h"

TDT4102::Point operator-(const TDT4102::Point& first, const TDT4102::Point& second) {
    return TDT4102::Point{first.x - second.x, first.y - second.y};
}

TDT4102::Point operator+(const TDT4102::Point& first, const TDT4102::Point& second){
    return TDT4102::Point{first.x + second.x, first.y + second.y};
}

TDT4102::Point operator*(const double& d, const TDT4102::Point& pointVector) {
    return TDT4102::Point{static_cast<int>(d * pointVector.x), static_cast<int>(d * pointVector.y)};
}

double operator*(const TDT4102::Point& first, const TDT4102::Point& second){
    return std::sqrt(first.x * second.x + first.y * second.y);
}


double getLen(const TDT4102::Point& point){
    return point * point;
}

double getDistance(const TDT4102::Point& first, const TDT4102::Point& second) {
    return getLen(second - first);
}

TDT4102::Point getTangent(const TDT4102::Point& point){
    return (10/getLen(point)) * point;
}
TDT4102::Point getNormal(const TDT4102::Point& point){
    TDT4102::Point tangent = getTangent(point);
    return TDT4102::Point{-tangent.y, tangent.x};
}