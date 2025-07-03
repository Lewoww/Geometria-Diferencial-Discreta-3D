#include "Vertex3d.h"

// Construtores
Vertex3d::Vertex3d(dividing x, dividing y, dividing z)
    : Point3d(x, y, z){}

Vertex3d::Vertex3d(const std::string& color, dividing x, dividing y, dividing z)
    : Point3d(color, x, y, z) {}

Vertex3d::Vertex3d(const Point3d& point)
    : Point3d(point){}
