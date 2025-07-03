#ifndef POINT3D_H
#define POINT3D_H

#include <string>
#include <iostream>
#include "dividing.h"

class Point3d {
private:
    std::string color;
    dividing x;
    dividing y;
    dividing z;

public:
    // Construtores
    Point3d(dividing x = dividing(0.0f, 1), dividing y = dividing(0.0f, 1), dividing z = dividing(0.0f, 1));
    Point3d(const std::string& color, dividing x, dividing y, dividing z);

    // Métodos auxiliares
    std::string getColor() const;
    void setColor(const std::string& color);

    dividing getX() const;
    void setX(const dividing& x);

    dividing getY() const;
    void setY(const dividing& y);

    dividing getZ() const;
    void setZ(const dividing& z);

    void printPoint() const;

    bool isEqual(const Point3d& a, const Point3d& b) const;
};

#endif
