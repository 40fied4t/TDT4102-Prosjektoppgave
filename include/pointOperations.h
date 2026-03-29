#pragma once

#include "AnimationWindow.h"
#include <cmath>

TDT4102::Point operator-(const TDT4102::Point& first, const TDT4102::Point& second);
TDT4102::Point operator+(const TDT4102::Point& first, const TDT4102::Point& second);
TDT4102::Point operator*(const double& d, const TDT4102::Point& pointVector);
double operator*(const TDT4102::Point& first, const TDT4102::Point& second);
double getLen(const TDT4102::Point& point);
double getDistance(const TDT4102::Point& first, const TDT4102::Point& second);
TDT4102::Point getNormal(const TDT4102::Point& point);
TDT4102::Point getTangent(const TDT4102::Point& point);