#include "Point3d.h"

// Construtor padrão
Point3d::Point3d(dividing x, dividing y, dividing z)
    : color("white"), x(x), y(y), z(z) {}

// Construtor com cor
Point3d::Point3d(const std::string& color, dividing x, dividing y, dividing z)
    : color(color), x(x), y(y), z(z) {}

// Getters e Setters
std::string Point3d::getColor() const {
    return color;
}

void Point3d::setColor(const std::string& color) {
    this->color = color;
}

dividing Point3d::getX() const {
    return x;
}

void Point3d::setX(const dividing& x) {
    this->x = x;
}

dividing Point3d::getY() const {
    return y;
}

void Point3d::setY(const dividing& y) {
    this->y = y;
}

dividing Point3d::getZ() const {
    return z;
}

void Point3d::setZ(const dividing& z) {
    this->z = z;
}

// Impressão
/*/
void Point3d::printPoint() const {
    std::cout << "Point3d(" << x << ", " << y << ", " << z << ") Color: " << color << std::endl;
}

// Comparação

bool Point3d::isEqual(const Point3d& a, const Point3d& b) const {
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.color == b.color);
}*/
