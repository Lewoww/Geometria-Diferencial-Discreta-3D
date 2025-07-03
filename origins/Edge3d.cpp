#include "Edge3d.h"
#include <cmath>

// Construtores
Edge3d::Edge3d(const std::string& color, const Vertex3d& p1, const Vertex3d& p2)
    : color(color), p1(p1), p2(p2) {
    computeLength();
}

Edge3d::Edge3d(const Vertex3d& p1, const Vertex3d& p2)
    : p1(p1), p2(p2), color("") {
    computeLength();
}

// Getters e setters
Vertex3d Edge3d::getP1() const {
    return p1;
}

void Edge3d::setP1(const Vertex3d& point) {
    p1 = point;
    computeLength();
}

Vertex3d Edge3d::getP2() const {
    return p2;
}

void Edge3d::setP2(const Vertex3d& point) {
    p2 = point;
    computeLength();
}

std::string Edge3d::getColor() const {
    return color;
}

void Edge3d::setColor(const std::string& newColor) {
    color = newColor;
}

// Cálculo de comprimento da aresta
void Edge3d::computeLength() {
    double dx = p2.getX().calculate() - p1.getX().calculate();
    double dy = p2.getY().calculate() - p1.getY().calculate();
    double dz = p2.getZ().calculate() - p1.getZ().calculate();

    _length = std::sqrt(dx * dx + dy * dy + dz * dz);
}

float Edge3d::getLength() const {
    return _length;
}
